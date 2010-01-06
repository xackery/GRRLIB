/*===========================================
    Loading obj file example.
============================================*/
#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

// Graphics
#include "gfx/Pyramid.h"
#include "font_png.h"


int main() {
    f32 modelRotX = 0.0f;
    f32 modelRotY = 0.0f;
    f32 modelRotZ = 0.0f;
    f32 camZ = 50.0f;
    u8 Amb = 0x80;
    f32 zlight = 0.0f;
    GRRLIB_Model* model;
    const char strCtl1[]   = "DPAD TO ROTATE MODEL";
    const char strCtl2[]   = "PLUS/MINUS TO ZOOM MODEL";
    const char strCredit[] = "BY NONAMENO/CRAYON FROM GRRLIB TEAM";
    
    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_texImg *tex_obj = GRRLIB_LoadTexture(Pyramid);

    GRRLIB_SetBackgroundColour(0x30, 0x30, 0x30, 0xFF);

    model = GRRLIB_ReadOBJ("sd:/data/head_chord.obj");

    while(1) {
        GRRLIB_Camera3dSettings(0.0f,0.0f,camZ, 0,1,0, 0,0,0);

        GRRLIB_3dMode(0.1, 1000, 45, 1, model->numtexcoords, model->numnormals);

        if(model->numtexcoords)
            GRRLIB_SetTexture(tex_obj, 0);

        GRRLIB_ObjectView(0, 0, 0, modelRotX, modelRotY, modelRotZ, 1, 1, 1);

        GRRLIB_InitLight(GX_LIGHT0, (guVector){-6, 0, zlight}, 0xFF0000FF);
        GRRLIB_InitLight(GX_LIGHT1, (guVector){ 6, 0, zlight}, 0x00FF00FF);
        GRRLIB_InitLight(GX_LIGHT2, (guVector){ 0,-6, zlight}, 0x0000FFFF);

        GRRLIB_LightSwitch(GX_LIGHT0|GX_LIGHT1|GX_LIGHT2, RGBA(Amb,Amb,Amb,0xFF), 0x808080FF, 0);

        Draw3dObj(model);

        GRRLIB_LightOff();
        GRRLIB_2dMode();
        GRRLIB_Printf(rmode->fbWidth/2.0 - (tex_font->tilew*strlen(strCtl1))/2.0, 20,
            tex_font, 0xFFFFFF33, 1, strCtl1);
        GRRLIB_Printf(rmode->fbWidth/2.0 - (tex_font->tilew*strlen(strCtl2))/2.0, 52,
            tex_font, 0xFFFFFF33, 1, strCtl2);
        GRRLIB_Printf(rmode->fbWidth/2.0 - (tex_font->tilew*strlen(strCredit))/2.0,
            rmode->efbHeight - tex_font->tileh - 7,
            tex_font, 0xFFFFFF33, 1, strCredit);

        GRRLIB_Render();

        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) modelRotY++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT)  modelRotY--;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN)  modelRotX++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP)    modelRotX--;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS)  camZ -= 0.3f;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS) camZ += 0.3f;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A)     zlight += 0.4f;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B)     zlight -= 0.4f;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_1 && WPAD_ButtonsHeld(0) & WPAD_BUTTON_2) {
            WPAD_Rumble(0, true);  // Rumble on
            GRRLIB_ScrShot("sd:/grrlib_3d.png");
            WPAD_Rumble(0, false); // Rumble off
        }
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    GRRLIB_DeleteObj(model);

    GRRLIB_FreeTexture(tex_obj);
    GRRLIB_FreeTexture(tex_font);

    exit(0);
}

