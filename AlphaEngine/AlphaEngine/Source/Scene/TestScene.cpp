#include "TestScene.h"
#include <cmath>
#include <iostream>
#include "../System/Scenemanager.h"
#include "TestScene2.h"
#include "MainMenu.h"
#include "../Utils/utils.h"
#include "../Enemy/ShadowEnemy.h"
#include "../Enemy/BatEnemy.h"
#include "../Enemy/ArcherEnemy.h"
#include "../Particles/ParticleManager.h"
#include "../Projectiles/SwordProjectile.h"

void TestScene::Load() {
    AEGfxReset();
    meshManager = MeshManager::GetInstance();
    map = Map::GetInstance();
    player = Player::GetInstance();
    map->LoadMap("Level1");

    // Load UI Resources
    Button::LoadResources();

    // We initialize the buttons here, but their positions will be 
    // updated every frame in the Update loop when paused.
    btnResume = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Resume");
    btnExit = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Quit to Menu");
}

void TestScene::Init() {
    player->GetTransform().SetPosition(300.0f, 900.0f);
    abilityHandler = new AbilityHandler();
    isPaused = false;

    // Spawn Enemies (Shadow, Bat, Archer)
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
    // 1. Toggle Pause State
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        isPaused = !isPaused;
    }

    if (!isPaused) {
        // --- NORMAL GAMEPLAY LOGIC ---
        f32 dt = (f32)AEFrameRateControllerGetFrameTime();

        ParticleManager::GetInstance()->Update(dt);
        player->HandleInput();
        player->Update(dt);
        if (abilityHandler) abilityHandler->AbilityUpdate(enemyList, dt);

        for (BaseEnemy* enemy : enemyList) {
            enemy->Update(dt);
        }

        // Enemy Projectile vs Player Collision
        for (BaseEnemy* enemy : enemyList) {
            std::vector<EnemyProjectile*>& enemyProjs = enemy->GetProjectiles();
            for (auto it = enemyProjs.begin(); it != enemyProjs.end(); ) {
                if (AABBOverlap((*it)->transform.GetPosition(), { 20,20 },
                    player->GetTransform().GetPosition(), { 35,35 })) {
                    player->TakeDamage(3);
                    delete* it;
                    it = enemyProjs.erase(it);
                }
                else { ++it; }
            }
        }

        // Player Projectile vs Enemies
        std::vector<BaseProjectile*>& projs = player->projectileList;
        for (auto it = projs.begin(); it != projs.end(); ) {
            bool hit = false;
            for (BaseEnemy* enemy : enemyList) {
                if (AABBOverlap((*it)->transform.GetPosition(), (*it)->transform.GetScale(),
                    enemy->transform.GetPosition(), enemy->transform.GetScale())) {
                    if (!enemy->IsReadyToDelete()) {
                        enemy->TakeDamage();
                        if (!dynamic_cast<SwordProjectile*>(*it)) hit = true;
                    }
                }
            }
            if (hit || (*it)->GetLifespan() <= 0.f) { delete* it; it = projs.erase(it); }
            else { ++it; }
        }

        // Cleanup Dead Enemies
        for (auto it = enemyList.begin(); it != enemyList.end(); ) {
            if ((*it)->IsReadyToDelete()) { delete* it; it = enemyList.erase(it); }
            else { ++it; }
        }

        // Win Condition
        if (player->GetCollidedTile().tileType == TILE_TYPE::BACKGROUND && enemyList.size() == 0) {
            SceneManager::ChangeScene(new TestScene2());
        }
    }
    else {
        // --- PAUSE MENU LOGIC ---
        AEVec2 pPos = player->GetTransform().GetPosition();

        // Sync the button hitboxes to the player's world position 
        // BEFORE checking for clicks.
        btnResume->SetPosition(pPos.x, pPos.y + 60.0f);
        btnExit->SetPosition(pPos.x, pPos.y - 60.0f);

        if (btnResume->IsClicked()) {
            isPaused = false;
        }
        if (btnExit->IsClicked()) {
            SceneManager::ChangeScene(new MainMenu());
        }
    }
}

void TestScene::Draw() {
    // 1. Standard World Rendering
    AEGfxSetBackgroundColor(0.4f, 0.4f, 0.4f);
    map->RenderMap();
    ParticleManager::GetInstance()->Draw();
    if (abilityHandler) abilityHandler->Draw();
    player->Draw(meshManager->GetMesh(CORNER_SQUARE));

    for (BaseEnemy* enemy : enemyList) {
        enemy->Draw();
    }

    if (isPaused) {
        AEVec2 pPos = player->GetTransform().GetPosition();

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);

        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);

        AEGfxSetTransparency(0.6f);

        // Transform and Draw the Box
        AEMtx33 scale, trans, final;
        float dimSize = 6000.0f;
        AEMtx33Scale(&scale, dimSize, dimSize);
        AEMtx33Trans(&trans, pPos.x - (dimSize / 2.0f), pPos.y - (dimSize / 2.0f));
        AEMtx33Concat(&final, &trans, &scale);

        AEGfxSetTransform(final.m);
        AEGfxMeshDraw(meshManager->GetMesh(CORNER_SQUARE), AE_GFX_MDM_TRIANGLES);

        AEGfxSetTransparency(1.0f);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

        // Draw Buttons
        btnResume->Draw();
        btnExit->Draw();
    }
}

void TestScene::Free() {
    player->DestroyInstance();
    if (abilityHandler) delete abilityHandler;

    // Cleanup UI
    if (btnResume) delete btnResume;
    if (btnExit) delete btnExit;

    // Cleanup Particles
    ParticleManager::GetInstance()->ClearAllParticles();

    for (BaseEnemy* enemy : enemyList) { delete enemy; }
    enemyList.clear();
}

void TestScene::Unload() {
    player = nullptr;
    abilityHandler = nullptr;
    meshManager = nullptr;
}