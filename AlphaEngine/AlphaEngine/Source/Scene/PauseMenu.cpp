#include "PauseMenu.h"
#include "../System/Scenemanager.h"
#include "MainMenu.h"
#include "TestScene.h"
#include "AEEngine.h"
#include <iostream>

extern bool gGameRunning;

void PauseMenu::Load() {
    //Ensure global button resources (texture/font/mesh) are loaded
    Button::LoadResources();

    // Top Button
    btnResume = new Button(0.0f, 0.0f, 200.0f, 50.0f, "Resume");

    // Middle Button
    btnSettings = new Button(0.0f, -60.0f, 200.0f, 50.0f, "Settings");

    // Bottom Button
    btnExit = new Button(0.0f, -120.0f, 200.0f, 50.0f, "Quit");
}

void PauseMenu::Init() {
    //leave this pls
}

void PauseMenu::Update() {

    if (btnResume->IsClicked()) {
        std::cout << "Play Pressed! Loading Game..." << std::endl;
        SceneManager::ChangeScene(new TestScene());
        return;
    }

    if (btnSettings->IsClicked()) {
        std::cout << "Settings Pressed (Feature not implemented yet)" << std::endl;
        // SceneManager::ChangeScene(new SettingsScene());
    }

    if (btnExit->IsClicked()) {
        std::cout << "Quit Pressed! Exiting..." << std::endl;
		SceneManager::ChangeScene(new MainMenu());
    }
}

void PauseMenu::Draw() {
    AEGfxSetCamPosition(0.0f, 0.0f);

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    btnResume->Draw();
    btnSettings->Draw();
    btnExit->Draw();
}

void PauseMenu::Free() {
    if (btnResume) delete btnResume;
    if (btnSettings) delete btnSettings;
    if (btnExit) delete btnExit;
}

void PauseMenu::Unload() {
    //leave this pls
}