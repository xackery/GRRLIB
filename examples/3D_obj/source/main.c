/*===========================================
    Loading obj file example.
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <wiiuse/wpad.h>
#include "gfx/Pyramid.h"
extern GXRModeObj *rmode;


int main() {
    float a=0;
    GRRLIB_Model* model;
    
    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_Settings.antialias = true;

    GRRLIB_texImg *tex_Pyr;
    tex_Pyr= GRRLIB_LoadTexture(Pyramid);

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);

    model = GRRLIB_ReadOBJ("sd:/data/couch.obj");

    while(1) {
    GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

        GRRLIB_3dMode(0.1, 1000, 45, 1, model->numtexcoords, model->numnormals);
        
        if(model->numtexcoords)
            GRRLIB_SetTexture(tex_Pyr, 0);


        GRRLIB_ObjectView(0,0,0, a,a*2,a*3);

        Draw3dObj(model);

        a+=0.5f;
        GRRLIB_Render();
    }
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    GRRLIB_DeleteObj(model);

    exit(0);
}

