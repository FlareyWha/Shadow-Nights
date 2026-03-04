#include "Button.h"
#include <iostream>
#include "MeshManager.h"

AEGfxTexture* Button::sprite = nullptr;
AEGfxVertexList* Button::pMesh = nullptr;
s8 Button::fontId = -1;;


Button::Button(float x, float y, float width, float height, const char* text)
    : x(x), y(y), w(width), h(height), label(text)
{
    if (!sprite || !pMesh) {
        LoadResources();
    }
}

void Button::Draw()
{
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    if (sprite) {
        AEGfxTextureSet(sprite, 0, 0);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    }

    AEMtx33 scale, trans, final;
    AEMtx33Scale(&scale, w, h);
    AEMtx33Trans(&trans, x, y);
    AEMtx33Concat(&final, &trans, &scale);
    AEGfxSetTransform(final.m);

    if (pMesh) AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

    if (fontId != -1) {
        float textW, textH;
        AEGfxGetPrintSize(fontId, label, 1.0f, &textW, &textH);

        float camX, camY;
        AEGfxGetCamPosition(&camX, &camY);

        float relativeX = x - camX;
        float relativeY = y - camY;

        float textX = (relativeX / 800.0f) - (textW / 2.0f);
        float textY = (relativeY / 450.0f) - (textH / 2.0f);

        AEGfxPrint(fontId, label, textX, textY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

bool Button::IsHovered()
{
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(AESysGetWindowHandle(), &mousePos);

    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);

    float worldMouseX = (mousePos.x - 800.0f) + camX;
    float worldMouseY = (450.0f - mousePos.y) + camY;

    return (worldMouseX > x - w / 2 && worldMouseX < x + w / 2 &&
        worldMouseY > y - h / 2 && worldMouseY < y + h / 2);
}


bool Button::IsClicked()
{
    return IsHovered() && AEInputCheckTriggered(AEVK_LBUTTON);
}


void Button::LoadResources() {
    if (!sprite) {
        sprite = AEGfxTextureLoad("Assets/Button/Default/TransParent center/panel-transparent-center-023.png");

        if (!sprite) std::cout << "CRITICAL ERROR: Texture Not Found! Check path." << std::endl;
    }

    if (fontId == -1) {
        fontId = AEGfxCreateFont("Assets/Fonts/ttf/monogram.ttf", 25);

        if (fontId == -1) std::cout << "CRITICAL ERROR: Font Not Found! Check path." << std::endl;
    }

    if (!pMesh) {
        pMesh = MeshManager::GetInstance()->GetMesh(TEXTURE_SQUARE);
    }
}


void Button::FreeResources() {
    if (sprite) {
        AEGfxTextureUnload(sprite);
        sprite = nullptr;
    }

    pMesh = nullptr;
}


void Button::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}