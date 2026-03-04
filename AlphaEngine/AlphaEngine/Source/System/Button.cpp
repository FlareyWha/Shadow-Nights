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
    // --- 1. Draw the Button Background (World Space) ---
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

    // --- 2. Draw the Text (Corrected for Camera) ---
    if (fontId != -1) {
        float textW, textH;
        AEGfxGetPrintSize(fontId, label, 1.0f, &textW, &textH);

        // Get the current camera position to calculate relative offset
        float camX, camY;
        AEGfxGetCamPosition(&camX, &camY);

        // Calculate where the button is RELATIVE to the camera
        float relativeX = x - camX;
        float relativeY = y - camY;

        // Convert that relative position to the -1 to 1 range for AEGfxPrint
        // Assuming 1600x900 resolution (800 and 450 are half-widths)
        float textX = (relativeX / 800.0f) - (textW / 2.0f);
        float textY = (relativeY / 450.0f) - (textH / 2.0f);

        // Use 9 arguments (added 1.0f for alpha)
        AEGfxPrint(fontId, label, textX, textY, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

bool Button::IsHovered()
{
    // 1. Get Mouse Pixel Position
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(AESysGetWindowHandle(), &mousePos);

    // 2. Get Camera Position (CRITICAL)
    // Since the buttons are in the World following the player, 
    // we need to know where the camera is looking.
    float camX, camY;
    AEGfxGetCamPosition(&camX, &camY);

    // 3. Convert Mouse to World Coordinates
    // Pixel (0,0) is top-left. World (0,0) is center.
    // Half-width = 800, Half-height = 450
    float worldMouseX = (mousePos.x - 800.0f) + camX;
    float worldMouseY = (450.0f - mousePos.y) + camY;

    // 4. AABB Collision Check
    // Check if the world mouse is within the button's world bounds
    return (worldMouseX > x - w / 2 && worldMouseX < x + w / 2 &&
        worldMouseY > y - h / 2 && worldMouseY < y + h / 2);
}


bool Button::IsClicked()
{
    //checks for left click
    return IsHovered() && AEInputCheckTriggered(AEVK_LBUTTON);
}


void Button::LoadResources() {
    // Load Texture
    if (!sprite) {
        sprite = AEGfxTextureLoad("Assets/Button/Default/TransParent center/panel-transparent-center-023.png");

		//for debugging
        if (!sprite) std::cout << "CRITICAL ERROR: Texture Not Found! Check path." << std::endl;
    }

    if (fontId == -1) {
        fontId = AEGfxCreateFont("Assets/Fonts/ttf/monogram.ttf", 25);

		//for debugging
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