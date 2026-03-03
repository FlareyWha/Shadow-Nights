#pragma once
#include "BaseProjectile.h"
#include "../Sprite/Sprite.h"

class EnemyProjectile : public BaseProjectile
{
public:
    EnemyProjectile(AEVec2 pos, AEVec2 dir);
    ~EnemyProjectile();

    void Update(float deltaTime) override;
    void Draw();

    // --- FUNCTION DECLARATIONS ---
    // These tell the compiler these functions exist, 
    // satisfying the requirements of BaseEnemy.cpp.
    bool IsDead();
    void Speed(float newSpeed);
    float Speed() const;
    float OriginalSpeed() const;

    bool isHoming = false;

private:
    Sprite* sprite = nullptr;
    float   oriSpeed;
    float   speed;
    float   trailTimer;
};