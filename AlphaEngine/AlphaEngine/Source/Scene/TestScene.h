#pragma once
#include "../Scene/BaseScene.h"
#include "../System/MeshManager.h"
#include "../System/Map.h"
#include "../Player/Player.h"
#include "../Abilities/AbilityHandler.h"
#include "../Enemy/BaseEnemy.h"
#include "../System/Button.h"
#include <vector>

class TestScene : public BaseScene {
public:
    virtual void Load() override;
    virtual void Init() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Free() override;
    virtual void Unload() override;

private:
    MeshManager* meshManager = nullptr;
    Map* map = nullptr;
    Player* player = nullptr;
    AbilityHandler* abilityHandler = nullptr;
    std::vector<BaseEnemy*> enemyList;

    // --- PAUSE STATE DATA ---
    bool isPaused = false;
    Button* btnResume = nullptr;
    Button* btnExit = nullptr;
};