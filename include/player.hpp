#ifndef PLAYER_H
#define PLAYER_H


#include<SDL_image.h>
//#include "world.hpp"
class World;
class  Tex_Manager;

typedef enum {
    DIR_NONE = -1,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP
} Direction;//玩家朝向

class Player
{
private:
    
public:
    int tile_x, tile_y;//瓦片坐标
    
    int px,py;//屏幕坐标
    float x,y;//移动的中间值，世界坐标
    float start_x, start_y;//开始移动的位置

    //渲染的区域
    SDL_Rect player_rect;

    //渲染到的矩形区域
    SDL_Rect hero_screen;

    //碰撞体积
    SDL_Rect collision_box;

    int facing;

    int move_frames;//8帧
    int move_frame_count;//计数帧
    int move_tile_count;

    bool moving;
    int w,h;
    bool battle_state;
    Player(/* args */);
    
    void player_update(World *world,Tex_Manager *tex);
    ~Player();

};
#endif 