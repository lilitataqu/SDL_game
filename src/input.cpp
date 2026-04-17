#include "world.hpp"
#include "game.hpp"
#include "tex_manager.hpp"
#include "player.hpp"
#include "config.hpp"
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

    //摄像机左上角在世界中的位置 摄像机大小 = 窗口大小
    world->camera.x = 0 ,world->camera.y = 0,
    //人物渲染的起始坐标，和px一样
    player->hero_screen.x = player->px - world->camera.x;
    player->hero_screen.y = player->py - world->camera.y - TILE_SIZE;

    player->moving = 0;
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
        world->get_tile(world->map->logicmap[ny][nx]).walkable) {
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
            
            // 清空
            this->moving = 0;
        }
    }
    if (world->get_tile(world->map->logicmap[tile_y][tile_x]).exit_id == 1)
        {
            //找到传送点瓦片起始坐标
            int i = 0;
            while (!(this->tile_x != world->map->portals[i].from_x &&
                   this->tile_y != world->map->portals[i].from_y) && i <= 10 )
            {
               i++;
            }
            
            world->mapup = true;
            if (world->mapup == true)
        {
            //更新英雄瓦片坐标再更新地图
            this->tile_x = world->map->portals[i].to_x;  // 起始瓦片位置
            this->tile_y = world->map->portals[i].to_y;
            world->map = &(world->maps[world->map->portals[i].map1][world->map->portals[i].map2]);
            map_update(world,this);
            world->mapup = false;
        }
            return;
        }
        if(world->get_tile(world->map->logicmap[tile_y][tile_x]).event_id == battle)
        {
            this->battle_state = true;
            tex->btl_bg[0].able = 1;
            tex->pokemon_tex[0].able = 1;
            tex->pokemon_tex[1].able = 1;
            tex->ui[0].able = 1;
        }
    return ;
}

void ui(Game *game)
{
    while (SDL_PollEvent(&(game->event)))
        {
            //相应菜单事件，如关闭游戏，打开设置
           switch (game->event.type)
            {
            case SDL_QUIT:
                game->running = 0;
                break;
            case SDL_KEYDOWN:
            // 添加ESC键退出功能
            if (game->event.key.keysym.sym == SDLK_ESCAPE) {
                game->running = 0;
            }
            break;
            case SDL_WINDOWEVENT:
                if(game->event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    float scale = fmin(game->event.window.data1 / (game->window_w * 1.0f),game->event.window.data2 / (game->window_h * 1.0f));
                    game->rect.w = scale * game->window_w ,game->rect.h = scale * game->window_h ;
                    game->rect.x = (game->event.window.data1 - game->rect.w) / 2;
                    game->rect.y = (game->event.window.data2 - game->rect.h) / 2;
                }
            default:
                break;
            }
        } 
}
