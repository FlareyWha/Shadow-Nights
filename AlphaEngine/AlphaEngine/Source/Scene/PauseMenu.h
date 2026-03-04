#pragma
#include "../../Source/Scene/BaseScene.h"
#include "../System/Button.h"

class PauseMenu : public BaseScene {
public:
    void Load() override;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Free() override;
    void Unload() override;

private:
    Button* btnResume = nullptr;
    Button* btnSettings = nullptr;
	Button* btnSave = nullptr;
	Button* btnLoad = nullptr;
    Button* btnExit = nullptr;
};