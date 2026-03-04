#include "EnemyProjectile.h"
#include "../System/CSVManager.h"
#include "../Particles/ParticleManager.h" 
#include "../Player/Player.h"
#include <cmath>

EnemyProjectile::EnemyProjectile(AEVec2 pos, AEVec2 dir)
    : BaseProjectile(pos, dir, true)
    , trailTimer(0.0f)
{
    oriSpeed = 400.0f;
    speed = oriSpeed;

    transform.SetScale(40.0f, 40.0f);
    transform.SetPosition(pos.x, pos.y);
    SetLifespan(3.0f);

    sprite = new Sprite(CSVManager::ReadInfo("shadoworb"));
    sprite->PlayAnimation("orb1", -1, 1.0f);
}

EnemyProjectile::~EnemyProjectile()
{
    if (sprite)
    {
        delete sprite;
        sprite = nullptr;
    }
    Exit();
}

void EnemyProjectile::Update(float deltaTime)
{
    if (GetLifespan() > 0.f)
    {
        if (!stopped)
        {
            SetLifespan(GetLifespan() - deltaTime);

            // Trail Logic
            trailTimer += deltaTime;
            if (trailTimer >= 0.05f)
            {
                ParticleManager::GetInstance()->SpawnParticle(
                    new BaseParticle(transform.GetPosition(), { 0.0f, 0.0f }, 0.3f, "shadoworb", "idle")
                );
                trailTimer = 0.0f;
            }

            // Homing Logic
            if (isHoming)
            {
                Player* player = Player::GetInstance();
                AEVec2 playerPos = player->GetTransform().GetPosition();
                AEVec2 myPos = transform.GetPosition();

                float dx = playerPos.x - myPos.x;
                float dy = playerPos.y - myPos.y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist > 0.0f)
                {
                    float perfectX = dx / dist;
                    float perfectY = dy / dist;
                    float turnSpeed = 3.0f;
                    AEVec2 currentDir = GetDirection();

                    float newDirX = currentDir.x + (perfectX - currentDir.x) * turnSpeed * deltaTime;
                    float newDirY = currentDir.y + (perfectY - currentDir.y) * turnSpeed * deltaTime;

                    float newMag = sqrt(newDirX * newDirX + newDirY * newDirY);
                    SetDirection({ newDirX / newMag, newDirY / newMag });
                }
            }

            speed = slowed ? (oriSpeed * 0.5f) : oriSpeed;

            AEVec2 position = transform.GetPosition();
            position.x += speed * GetDirection().x * deltaTime;
            position.y += speed * GetDirection().y * deltaTime;

            transform.SetRotation(atan2f(GetDirection().y, GetDirection().x) + (3.14159f * 1.0f));            
            transform.SetPosition(position.x, position.y);

            if (sprite) sprite->Update();
        }
    }
}

void EnemyProjectile::Draw()
{
    if (sprite) sprite->Render(transform);
}

bool EnemyProjectile::IsDead()
{
    return GetLifespan() <= 0.0f;
}

// Sets a new movement speed
void EnemyProjectile::Speed(float newSpeed)
{
    this->speed = newSpeed;
}

// Returns the current speed
float EnemyProjectile::Speed() const
{
    return this->speed;
}

// Returns the original speed (before slows/modifiers)
float EnemyProjectile::OriginalSpeed() const
{
    return this->oriSpeed;
}