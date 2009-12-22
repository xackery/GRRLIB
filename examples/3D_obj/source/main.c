/*===========================================
    Loading obj file example.
============================================*/
#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

// Graphics
#include "gfx/Pyramid.h"
#include "font_png.h"


extern GXRModeObj *rmode;


int main() {
    f32 modelRotX = 0.0f;
    f32 modelRotY = 0.0f;
    f32 modelRotZ = 0.0f;
    f32 camZ = 50.0f;
    GRRLIB_Model* model;
    const char strCtl1[]   = "DPAD TO ROTATE MODEL";
    const char strCtl2[]   = "PLUS/MINUS TO ZOOM MODEL";
    const char strCredit[] = "BY NONAMENO/CRAYON FROM GRRLIB TEAM";
    
    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_texImg *tex_Pyr;
    tex_Pyr= GRRLIB_LoadTexture(Pyramid);

    GRRLIB_SetBackgroundColour(0x30, 0x30, 0x30, 0xFF);

    model = GRRLIB_ReadOBJ("sd:/data/head_chord.obj");

    while(1) {
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  exit(0);
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) modelRotY++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT)  modelRotY--;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN)  modelRotX++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP)    modelRotX--;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS)  camZ -= 0.3f;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS) camZ += 0.3f;

        GRRLIB_Camera3dSettings(0.0f,0.0f,camZ, 0,1,0, 0,0,0);

        GRRLIB_3dMode(0.1, 1000, 45, 1, model->numtexcoords, model->numnormals);

        if(model->numtexcoords)
            GRRLIB_SetTexture(tex_Pyr, 0);

        GRRLIB_ObjectView(0, 0, 0, modelRotX, modelRotY, modelRotZ);

        Draw3dObj(model);

        GRRLIB_2dMode();
        GRRLIB_Printf(rmode->fbWidth/2.0 - (tex_font->tilew*strlen(strCtl1))/2.0, 20,
            tex_font, 0xFFFFFF33, 1, strCtl1);
        GRRLIB_Printf(rmode->fbWidth/2.0 - (tex_font->tilew*strlen(strCtl2))/2.0, 52,
            tex_font, 0xFFFFFF33, 1, strCtl2);
        GRRLIB_Printf(rmode->fbWidth/2.0 - (tex_font->tilew*strlen(strCredit))/2.0,
            rmode->efbHeight - tex_font->tileh - 7,
            tex_font, 0xFFFFFF33, 1, strCredit);

        GRRLIB_Render();
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    GRRLIB_DeleteObj(model);

    GRRLIB_FreeTexture(tex_font);

    exit(0);
}

