#include "pch.h"
#include "TTFont.h"
#include <stdio.h>
#include <stdlib.h>
#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h"

TTFont::TTFont(const char* path) {

    long size;
    unsigned char* fontBuffer;

    FILE* fontFile = fopen(path, "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */

    fontBuffer = (unsigned char*)malloc(size);

    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("failed\n");
    }

}