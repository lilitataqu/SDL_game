#include "player.hpp"
#include "world.hpp"
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

    //碰撞盒
    collision_box = {
        px,py,w,h
    };
}


void map_update(World *world,Player *player)
{
     //小数，计算时用到的世界坐标
    player->x = player->tile_x * TILE_SIZE;
    player->y = player->tile_y * TILE_SIZE;
    //int 屏幕坐标
    player->px = player->tile_x * TILE_SIZE;
    player->py =  player->tile_y * TILE_SIZE;
    //帧坐标
    player->start_x = player->x;
    player->start_y = player->y;
    player->move_frame_count = 0;
    player->moving = false;

    // 根据玩家位置重新计算摄像机
    world->camera.x = player->px - (world->camera.w - player->w) / 2;
    world->camera.y = player->py - (world->camera.h - player->h) / 2;
    // 钳位
    if (world->camera.x < 0) world->camera.x = 0;
    if (world->camera.y < 0) world->camera.y = 0;
    if (world->camera.x > world->map->map_w * TILE_SIZE - world->camera.w)
        world->camera.x = world->map->map_w * TILE_SIZE - world->camera.w;
    if (world->camera.y > world->map->map_h * TILE_SIZE - world->camera.h)
        world->camera.y = world->map->map_h * TILE_SIZE - world->camera.h;
    //人物渲染的起始坐标，和px一样
    player->hero_screen.x = player->px - world->camera.x;
    player->hero_screen.y = player->py - world->camera.y - TILE_SIZE;
    player->collision_box.x = player->px;
    player->collision_box.y = player->py;
    return ;
}

Direction get_dir_from_key(const Uint8* keys)
{
    if (keys[SDL_SCANCODE_UP ]|| keys[SDL_SCANCODE_W])    return DIR_UP;
    if (keys[SDL_SCANCODE_DOWN ]|| keys[SDL_SCANCODE_S])  return DIR_DOWN;
    if (keys[SDL_SCANCODE_LEFT ] || keys[SDL_SCANCODE_A])  return DIR_LEFT;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) return DIR_RIGHT;

    return DIR_NONE;
}

void dir_to_offset(Direction dir, int* dx, int* dy)
{
    *dx = 0;
    *dy = 0;

    switch (dir) {
    case DIR_UP:    *dy = -1; break;
    case DIR_DOWN:  *dy =  1; break;
    case DIR_LEFT:  *dx = -1; break;
    case DIR_RIGHT: *dx =  1; break;
    default: break;
    }
}


void Player::player_update(World *world,Tex_Manager *tex)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    Direction dir = get_dir_from_key(keys);

    // 永远允许更新朝向
    if (dir != DIR_NONE)
        this->facing = dir;

    // 只有不在移动中，才尝试移动
    if (!this->moving && dir != DIR_NONE) {
        int dx, dy;
        dir_to_offset(dir, &dx, &dy);

        int nx = this->tile_x + dx;
        int ny = this->tile_y + dy;

        if (nx >= 0 && nx < world->map->map_w &&
        ny >= 0 && ny < world->map->map_h &&
        world->get_tile(world->map->logic_map[ny][nx]).walkable) {
            this->moving = 1;
            
            // 记住开始移动坐标，开始移动
            this->start_x = this->x;
            this->start_y = this->y;
            
            // 第0帧
            this->move_frame_count = 0;

            this->tile_x = nx;
            this->tile_y = ny;

            this->px = nx * TILE_SIZE;
            this->py = ny * TILE_SIZE;
        }
    }

    // 如果正在移动，执行插值
    if (this->moving) 
    {
        // 第一帧
        this->move_frame_count++;
        
        // 8帧间的差距
        float progress = (float)this->move_frame_count / (float)this->move_frames;
        
        
        this->x = this->start_x + (this->px - this->start_x) * progress;
        this->y = this->start_y + (this->py - this->start_y) * progress;
        // 摄像头跟随
        world->camera.x = (int)(this->x - (world->camera.w - this->w)/2);
        world->camera.y = (int)(this->y - (world->camera.h - this->h)/2);
        
        // 摄像头范围
        if (world->camera.x < 0)
            world->camera.x = 0;
        if (world->camera.x > world->map->map_w*TILE_SIZE - world->camera.w)
            world->camera.x = world->map->map_w*TILE_SIZE - world->camera.w;
        if (world->camera.y < 0)
            world->camera.y = 0;
        if (world->camera.y > world->map->map_h*TILE_SIZE - world->camera.h)
            world->camera.y = world->map->map_h*TILE_SIZE - world->camera.h;

        // Update hero screen position for rendering
        this->hero_screen.x = (int)(this->x - world->camera.x);
        this->hero_screen.y = (int)(this->y - world->camera.y) - TILE_SIZE;
        // Check if movement is complete
        if (this->move_frame_count >= this->move_frames)
        {
            // 将屏幕坐标更新到世界坐标
            this->x = this->px;
            this->y = this->py;
            
            
            // 移动摄像头
            world->camera.x = (this->px - (world->camera.w - this->w)/2);
            world->camera.y = (this->py - (world->camera.h - this->h)/2);
            
            if (world->camera.x < 0)
                world->camera.x = 0;
            if (world->camera.x > world->map->map_w*TILE_SIZE - world->camera.w)
                world->camera.x = world->map->map_w*TILE_SIZE - world->camera.w;
            if (world->camera.y < 0)
                world->camera.y = 0;
            if (world->camera.y > world->map->map_h*TILE_SIZE - world->camera.h)
                world->camera.y = world->map->map_h*TILE_SIZE - world->camera.h;
            
            this->hero_screen.x = this->px - world->camera.x;
            this->hero_screen.y = this->py - world->camera.y - TILE_SIZE;
            //碰撞盒
            collision_box.x = px ;
            collision_box.y = py ;
            // 清空
            this->moving = 0;
            //检测事件
            for (auto& p : world->map->portals)
            {   //矩形检测，并且有朝向要求
                if (SDL_HasIntersection(&collision_box, &p.rect) && p.direction == facing)
                {
        
                    tile_x = p.target_x;
                    tile_y = p.target_y;
                    world->map = world->maps[p.target_map].get();
                    map_update(world,this);
                    break;
                }
            }
        }
    }
   
        
    return ;
}

Player::~Player() {}
