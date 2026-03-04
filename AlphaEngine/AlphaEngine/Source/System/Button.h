#pragma once
#include "AEEngine.h"

class Button {
public:
    Button(float x, float y, float w, float h, const char* text);

    void Draw();
    bool IsHovered(); 
    bool IsClicked(); 
    void SetPosition(float x, float y);

    static void LoadResources(); 
    static void FreeResources();

private:
    float x, y, w, h;  
    const char* label; 

    static AEGfxTexture* sprite;
    static AEGfxVertexList* pMesh; 
    static s8 fontId;
};