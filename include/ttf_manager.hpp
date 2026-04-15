#ifndef TTF_MANAGER_H
#define TTF_MANAGER_H
#include <SDL_ttf.h>
TTF_Font *font;
SDL_Texture* CreateTextTexture(SDL_Renderer* rdr,
                               TTF_Font* font,
                               const char* text,
                               SDL_Color color);
#endif