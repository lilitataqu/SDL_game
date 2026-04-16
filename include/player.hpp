#pragma once
#include<SDL_image.h>
#include "world.hpp"
//struct World;
class  Tex_Manager;

typedef enum {
    DIR_NONE = -1,
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;//玩家朝向

class Player
{
private:
    
public:
    int tile_x, tile_y;
    
    int px,py;
    float x,y;
    float start_x, start_y;

   
    SDL_Rect hero_screen;

    int facing;

    int move_frames;
    int move_frame_count;

    bool moving;
    int w,h;
    bool battle_state;
    Player(/* args */);

    ~Player();

    void player_update(World *world,Player *player,Tex_Manager *tex);
};
