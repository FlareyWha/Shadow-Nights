#include "ParticleManager.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance() {
    if (instance == nullptr) instance = new ParticleManager();
    return instance;
}

void ParticleManager::Destroy() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

ParticleManager::ParticleManager() {}

ParticleManager::~ParticleManager() {
    ClearAllParticles();
}

void ParticleManager::Update(float deltaTime)
{
    // Use an iterator to safely remove items while looping
    for (auto it = particles.begin(); it != particles.end(); )
    {
        (*it)->Update(deltaTime);

        // Check if the particle should be destroyed
        if (!(*it)->IsAlive())
        {
            delete* it;               // 1. Free the heap memory
            it = particles.erase(it);  // 2. Remove the pointer from the list
        }
        else
        {
            ++it; // Only increment if we didn't erase
        }
    }
}

void ParticleManager::Draw() {
    for (auto& particle : particles) {
        particle->Draw();
    }
}

void ParticleManager::SpawnParticle(BaseParticle* newParticle) {
    if (newParticle) particles.push_back(newParticle);
}

void ParticleManager::ClearAllParticles() {
    for (auto& particle : particles) {
        delete particle;
    }
    particles.clear();
}

