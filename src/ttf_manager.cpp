#include<SDL_ttf.h>
SDL_Texture* CreateTextTexture(SDL_Renderer* rdr,
                               TTF_Font* font,
                               const char* text,
                               SDL_Color color)
{
    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(font, text, color);

    if(!surface)
    {
        printf("Text surface error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture* tex =
        SDL_CreateTextureFromSurface(rdr, surface);

    SDL_FreeSurface(surface);

    return tex;
}