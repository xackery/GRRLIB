/*===========================================
        TrueType Font demo
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <ogc/pad.h>
#include <ogc/lwp_watchdog.h>   // Needed for gettime and ticks_to_millisecs

// Font
#include "FreeMonoBold_ttf.h"

// Prototype
static u8 CalculateFrameRate();
static bool ScreenShot();

int main(int argc, char **argv) {
    char FPS[255] = "";
    bool ShowFPS = false;

    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();

    // Initialise the Wii Remotes
    PAD_Init();

    // Load the font from memory
    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    // Create an empty texture to store a copy of the screen
    GRRLIB_texImg *CopiedImg = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);

    // Fill a table with characters
    u32 i, n = 0;
    wchar_t charTable[450];
    for(i=33; i<=126; i++) { // 0 to 93
        charTable[n++] = i;
    }
    for(i=161; i<=516; i++) { // 94 to 449
        charTable[n++] = i;
    }

    // Seed the random-number generator with current time so that
    // the numbers will be different every time we run.
    srand(time(NULL));

    wchar_t Letter[2] = L""; // A character + terminal NULL

    // To have a cool effect anti-aliasing is turned on
    GRRLIB_Settings.antialias = true;

    // Black background
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    // Loop forever
    while(1) {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
        Letter[0] = charTable[rand() % 449];
        GRRLIB_PrintfTTFW(rand() % rmode->fbWidth - 50,
                         rand() % rmode->efbHeight - 50,
                         myFont,
                         Letter,
                         rand() % 180 + 20,
                         ((rand() % 0xFFFFFF) << 8) | 0xFF);
        GRRLIB_Screen2Texture(0, 0, CopiedImg, false);

        if(ShowFPS) {
            sprintf(FPS, "Current FPS: %d", CalculateFrameRate());
            GRRLIB_PrintfTTF(500+1, 25+1, myFont, FPS, 12, 0x000000FF);
            GRRLIB_PrintfTTF(500, 25, myFont, FPS, 12, 0xFFFFFFFF);
        }

        PAD_ScanPads();  // Scan the Wii Remotes

        if (PAD_ButtonsDown(0) & PAD_BUTTON_START) {
            break;
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
            GRRLIB_Screen2Texture(0, 0, CopiedImg, false);
        }
        if (PAD_ButtonsDown(0) & PAD_BUTTON_B) {
            ShowFPS = !ShowFPS;
        }
        if(PAD_ButtonsHeld(0) & PAD_TRIGGER_L && PAD_ButtonsHeld(0) & PAD_TRIGGER_R) {
		PAD_ControlMotor(0, PAD_MOTOR_RUMBLE);
            ScreenShot();    // Needs to be after GRRLIB_Render()
		PAD_ControlMotor(0, PAD_MOTOR_STOP);
        }

        GRRLIB_Render();  // Render the frame buffer to the TV
    }

    GRRLIB_FreeTexture(CopiedImg);
    GRRLIB_FreeTTF(myFont);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}

/**
 * This function calculates the number of frames we render each second.
 * @return The number of frames per second.
 */
static u8 CalculateFrameRate() {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}

/**
 * Create a PNG screenshot on the root of the SD card with a timestamp.
 * @return bool true=everything worked, false=problems occurred.
 */
static bool ScreenShot() {
    char path[255];
    time_t now = time(NULL);
    struct tm *ti = localtime(&now);
    sprintf(path, "sd:/grrlib_ttf_%d%02d%02d%02d%02d%02d.png",
        ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
    return GRRLIB_ScrShot(path);
}
