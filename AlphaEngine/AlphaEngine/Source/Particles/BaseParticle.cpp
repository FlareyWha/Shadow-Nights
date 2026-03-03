#include "BaseParticle.h"
#include "../System/CSVManager.h"

BaseParticle::BaseParticle(AEVec2 pos, AEVec2 vel, float life, std::string spriteName, std::string startAnim)
    : velocity(vel), lifespan(life), maxLifespan(life), isAlive(true)
{
    transform.SetPosition(pos.x, pos.y);
    transform.SetScale(25.0f, 25.0f);

    sprite = new Sprite(CSVManager::ReadInfo(spriteName));
    sprite->PlayAnimation(startAnim, -1, 1.0f);
}

BaseParticle::~BaseParticle() {
    if (sprite) {
        delete sprite;
        sprite = nullptr;
    }
}

void BaseParticle::Update(float deltaTime) {
    if (!isAlive) return;

    AEVec2 pos = transform.GetPosition();
    pos.x += velocity.x * deltaTime;
    pos.y += velocity.y * deltaTime;
    transform.SetPosition(pos.x, pos.y);

    if (sprite) sprite->Update();

    lifespan -= deltaTime;
    if (lifespan <= 0.0f) isAlive = false;
}

void BaseParticle::Draw() {
    if (!isAlive || !sprite) return;

    float alpha = lifespan / maxLifespan;
    sprite->Render(transform, alpha);
}