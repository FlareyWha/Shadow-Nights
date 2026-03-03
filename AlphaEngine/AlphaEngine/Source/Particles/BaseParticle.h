#pragma once
#include "AEEngine.h"
#include "../Sprite/Sprite.h"
#include "../Player/Transform2D.h"
#include <string>

class BaseParticle
{
protected:
    AEVec2      velocity;
    float       lifespan;
    float       maxLifespan;
    Transform2D transform;
    Sprite* sprite;
    bool        isAlive;

public:
    BaseParticle(AEVec2 pos, AEVec2 vel, float life, std::string spriteName, std::string startAnim);
    virtual ~BaseParticle();

    virtual void Update(float deltaTime);
    virtual void Draw();

    bool IsAlive() const { return isAlive; }
};