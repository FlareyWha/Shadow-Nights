#include "AEEngine.h"
#include "System/SceneManager.h"
#include "System/MeshManager.h"
#include "System/Map.h"
#include "Scene/TestScene.h"
#include "Scene/MainMenu.h"
#include "System/SettingsManager.h"
#include <iostream>
#include <vector>
#include <string>
#include "Particles/ParticleManager.h"

bool gGameRunning = true;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) 
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 1. Initialize
    if (AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL) == 0) {
        return 0;
    }
   // _CrtSetBreakAlloc(8067);

    //Init settings
    std::vector<std::string> settingsInfo = CSVManager::ReadSettingsInfo();
    SettingsManager* sManager = SettingsManager::GetInstance(settingsInfo);

    // 2. Set Scene
    SceneManager::ChangeScene(new MainMenu());

    // 3. Game Loop
    while (gGameRunning) {

        AESysFrameStart();

        // check if forcing the application to quit
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = false;

        //Check inputs for settings
        sManager->CheckSettingsInputs();

        float dt = (float)AEFrameRateControllerGetFrameTime();
        ParticleManager::GetInstance()->Update(dt);

        SceneManager::Update();

        AEGfxStart();
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
        SceneManager::Draw();
        ParticleManager::GetInstance()->Draw();

        AESysFrameEnd();
    }

    // 4. Cleanup
    SceneManager::Exit();
    MeshManager::DestroyInstance();
    Map::DestroyInstance();
    Button::FreeResources();
    SettingsManager::DestroyInstance();
    sManager = nullptr;
    ParticleManager::Destroy();
    AESysExit();

    return 1;
}