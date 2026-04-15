#ifndef RENDER_H
#define RENDER_H
#include "game.hpp"
#include "tex_manager.hpp"
void Render_Init(Game *game);
void draw_map(Game *game);
void draw_bg(Game *game,Tex_Manager *tex);
void draw(Game *game , Tex_Manager *tex);
#endif