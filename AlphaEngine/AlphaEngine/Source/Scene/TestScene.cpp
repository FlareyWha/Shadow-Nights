#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"
#include "../Utils/utils.h"
#include "../Enemy/ShadowEnemy.h"
#include "../Enemy/BatEnemy.h"
#include "../Enemy/ArcherEnemy.h"
#include "../Particles/ParticleManager.h" // Added include

void TestScene::Load() {
    AEGfxReset();
    meshManager = MeshManager::GetInstance();
    map = Map::GetInstance();
    player = Player::GetInstance();
    map->LoadMap("Level1");
}

void TestScene::Init() {
    player->GetTransform().SetPosition(300.0f, 900.0f);
    abilityHandler = new AbilityHandler();

    // Spawn Enemies
    int numberOfEnemies = 3;
    float startX = 600.0f;
    float spacing = 300.0f;

    for (int e = 0; e < numberOfEnemies; e++) {
        BaseEnemy* newEnemy = new ShadowEnemy();
        BaseEnemy* myBat = new BatEnemy();
        BaseEnemy* myArcher = new ArcherEnemy();

        float spawnX = startX + (e * spacing);

        newEnemy->Init(spawnX, 300.0f, 85.0f, 60.0f, "shadow1");
        enemyList.push_back(newEnemy);

        myBat->Init(spawnX, 700.0f, 85.0f, 60.0f, "batenemy");
        enemyList.push_back(myBat);

        myArcher->Init(spawnX + 150.0f, 300.0f, 85.0f, 60.0f, "sniperenemy");
        enemyList.push_back(myArcher);
    }
}

void TestScene::Update() {
    f32 dt = (f32)AEFrameRateControllerGetFrameTime();

    player->HandleInput();
    player->Update(dt);
    if (abilityHandler) abilityHandler->AbilityUpdate(enemyList, dt);

    // Update Enemies
    for (BaseEnemy* enemy : enemyList) {
        enemy->Update(dt);
    }

    // --- PROJECTILE COLLISION & CLEANUP ---
    for (BaseEnemy* enemy : enemyList) {
        std::vector<EnemyProjectile*>& enemyProjs = enemy->GetProjectiles();
        for (auto it = enemyProjs.begin(); it != enemyProjs.end(); ) {
            EnemyProjectile* proj = *it;

            // AABB Collision with Player
            if (AABBOverlap(proj->transform.GetPosition(), { 20,20 },
                player->GetTransform().GetPosition(), { 35,35 })) {
                player->TakeDamage(10);
                delete proj; // This triggers EnemyProjectile destructor (cleans sprite)
                it = enemyProjs.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    // Player Projectile vs Enemies
    std::vector<BaseProjectile*>& projs = player->projectileList;
    for (auto it = projs.begin(); it != projs.end(); ) {
        BaseProjectile* pProj = *it;
        bool destroyProjectile = false;

        for (BaseEnemy* enemy : enemyList) {
            if (AABBOverlap(pProj->transform.GetPosition(), pProj->transform.GetScale(),
                enemy->transform.GetPosition(), enemy->transform.GetScale())) {
                if (!enemy->IsReadyToDelete()) {
                    enemy->TakeDamage();
                    if (!dynamic_cast<SwordProjectile*>(pProj)) {
                        destroyProjectile = true;
                        break;
                    }
                }
            }
        }

        if (destroyProjectile || pProj->GetLifespan() <= 0.f) {
            delete pProj;
            it = projs.erase(it);
        }
        else {
            ++it;
        }
    }

    // Remove Dead Enemies
    for (auto it = enemyList.begin(); it != enemyList.end(); ) {
        if ((*it)->IsReadyToDelete()) {
            delete* it;
            it = enemyList.erase(it);
        }
        else {
            ++it;
        }
    }

    // Win Condition
    if (player->GetCollidedTile().tileType == TILE_TYPE::BACKGROUND && enemyList.size() == 0) {
        SceneManager::ChangeScene(new TestScene2());
    }
}

void TestScene::Draw() {
    AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);
    map->RenderMap();

    // Draw Particles behind entities but in front of map
    ParticleManager::GetInstance()->Draw();

    if (abilityHandler) abilityHandler->Draw();
    player->Draw(meshManager->GetMesh(CORNER_SQUARE));

    for (BaseEnemy* enemy : enemyList) {
        enemy->Draw();
    }
}

void TestScene::Free() {
    player->DestroyInstance();
    if (abilityHandler) delete abilityHandler;

    // --- SCENE CLEANUP ---
    // Remove all particles so they don't appear in the next level
    ParticleManager::GetInstance()->ClearAllParticles();

    for (BaseEnemy* enemy : enemyList) {
        delete enemy;
    }
    enemyList.clear();
}

void TestScene::Unload() {
    player = nullptr;
    abilityHandler = nullptr;
    meshManager = nullptr;
}