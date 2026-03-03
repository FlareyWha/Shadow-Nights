#pragma once
#include <list>
#include "BaseParticle.h"

class ParticleManager
{
private:
    std::list<BaseParticle*> particles;
    static ParticleManager* instance;
    ParticleManager();

public:
    static ParticleManager* GetInstance();
    static void Destroy();
    ~ParticleManager();

    void Update(float deltaTime);
    void Draw();
    void SpawnParticle(BaseParticle* newParticle);
    void ClearAllParticles();
};