/*------------------------------------------------------------------------------
Copyright (c) 2009 The GRRLIB Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------*/

#include <grrlib.h>

/**
 * Make a snapshot of the screen in a texture WITHOUT ALPHA LAYER.
 * @param posx top left corner of the grabbed part.
 * @param posy top left corner of the grabbed part.
 * @param tex A pointer to a texture representing the screen or NULL if an error occurs.
 * @param clear When this flag is set to true, the grabbed part is cleared while copying.
 */
void  GRRLIB_Screen2Texture (int posx, int posy, GRRLIB_texImg *tex, bool clear) {
    if(tex->data != NULL) {
        GX_SetTexCopySrc(posx, posy, tex->w, tex->h);
        GX_SetTexCopyDst(tex->w, tex->h, GX_TF_RGBA8, GX_FALSE);
        GX_CopyTex(tex->data, clear);
        GX_PixModeSync();
        GRRLIB_FlushTex(tex);
    }
}

/**
 * Start GX Compositing Process.
 */
void GRRLIB_CompoStart (void){
	GX_SetPixelFmt(GX_PF_RGBA6_Z24,GX_ZC_LINEAR);
        GX_PokeAlphaRead(GX_READ_NONE);

}

/**
 * End GX Compositing Process (Make a snapshot of the screen in a texture WITH ALPHA LAYER).
 * @param posx top left corner of the grabbed part.
 * @param posy top left corner of the grabbed part.
 * @param tex A pointer to a texture representing the screen or NULL if an error occurs.
 * @info EFB is cleared after this function.
 */
void GRRLIB_CompoEnd(int posx, int posy, GRRLIB_texImg *tex){
	GRRLIB_Screen2Texture(posx, posy, tex, FALSE);

        GX_SetTexCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
        GX_SetTexCopyDst(rmode->fbWidth, rmode->efbHeight, GX_TF_RGBA8, GX_FALSE);
        GX_CopyTex(TrashTex, GX_TRUE);
        GX_PixModeSync();
	
        if (rmode->aa)  GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR) ;
        else            GX_SetPixelFmt(GX_PF_RGB8_Z24  , GX_ZC_LINEAR) ;

}
