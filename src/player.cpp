#include "player.hpp"
#include "config.hpp"
Player::Player() {
    // 初始化英雄位置和属性
    this->w = 32;
    this->h = 64;
    this->tile_x = 12;  // 起始瓦片位置
    this->tile_y = 9;
     //小数，计算时用到的世界坐标
    this->x = this->tile_x * TILE_SIZE;
    this->y = this->tile_y * TILE_SIZE;
    //int 屏幕坐标
    this->px = this->tile_x * TILE_SIZE;
    this->py =  this->tile_y * TILE_SIZE;
    //帧坐标
    this->start_x = this->x;
    this->start_y = this->y;

    this->move_frames = MOVE_FRAMES;
    this->move_frame_count = 0;
    this->moving = false;
    this->facing = DIR_DOWN;

    //人物在屏幕的大小
    this->hero_screen.w =  this->w;
    this->hero_screen.h =  this->h;
}
Player::~Player() {}
