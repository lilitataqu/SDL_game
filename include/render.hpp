#ifndef RENDER_H
#define RENDER_H
#include "game.hpp"
#include "tex_manager.hpp"
void Render_Init(Game *game,World *world,Player *player);
void draw_map(Game *game,World *world,Player *player,Tex_Manager *tex);
void draw_bg(Game *game,Tex_Manager *tex);
void draw_pokemon_btl(Game *game , Tex_Manager *tex);
void draw_ui(Game *game , Tex_Manager *tex);
void draw(Game *game , Tex_Manager *tex,World *world,Player *player);

#endif