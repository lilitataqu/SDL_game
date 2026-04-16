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


void Player::player_update(World *world,Player *player,Tex_Manager *tex)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    Direction dir = get_dir_from_key(keys);

    // 永远允许更新朝向
    if (dir != DIR_NONE)
        player->facing = dir;

    // 只有不在移动中，才尝试移动
    if (!player->moving && dir != DIR_NONE) {
        int dx, dy;
        dir_to_offset(dir, &dx, &dy);

        int nx = player->tile_x + dx;
        int ny = player->tile_y + dy;

        if (world->map->logicmap.logic[ny][nx].walkable) {
            player->moving = 1;
            
            // 记住开始移动坐标，开始移动
            player->start_x = player->x;
            player->start_y = player->y;
            
            // 第0帧
            player->move_frame_count = 0;

            player->tile_x = nx;
            player->tile_y = ny;

            player->px = nx * TILE_SIZE;
            player->py = ny * TILE_SIZE;
        }
    }

    // 如果正在移动，执行插值
    if (player->moving) 
    {
        // 第一帧
        player->move_frame_count++;
        
        // 8帧间的差距
        float progress = (float)player->move_frame_count / (float)player->move_frames;
        
        
        player->x = player->start_x + (player->px - player->start_x) * progress;
        player->y = player->start_y + (player->py - player->start_y) * progress;
        // 摄像头跟随
        world->camera.x = (int)(player->x - (world->camera.w - player->w)/2);
        world->camera.y = (int)(player->y - (world->camera.h - player->h)/2);
        
        // 摄像头范围
        if (world->camera.x < 0)
            world->camera.x = 0;
        if (world->camera.x > world->map->map_w - world->camera.w)
            world->camera.x = world->map->map_w - world->camera.w;
        if (world->camera.y < 0)
            world->camera.y = 0;
        if (world->camera.y > world->map->map_h - world->camera.h)
            world->camera.y = world->map->map_h - world->camera.h;

        // Update hero screen position for rendering
        player->hero_screen.x = (int)(player->x - world->camera.x);
        player->hero_screen.y = (int)(player->y - world->camera.y) - TILE_SIZE;
        // Check if movement is complete
        if (player->move_frame_count >= player->move_frames)
        {
            // 将屏幕坐标更新到世界坐标
            player->x = player->px;
            player->y = player->py;
            
            
            // 移动摄像头
            world->camera.x = (player->px - (world->camera.w - player->w)/2);
            world->camera.y = (player->py - (world->camera.h - player->h)/2);
            
            if (world->camera.x < 0)
                world->camera.x = 0;
            if (world->camera.x > world->map->map_w - world->camera.w)
                world->camera.x = world->map->map_w - world->camera.w;
            if (world->camera.y < 0)
                world->camera.y = 0;
            if (world->camera.y > world->map->map_h - world->camera.h)
                world->camera.y = world->map->map_h - world->camera.h;
            
            player->hero_screen.x = player->px - world->camera.x;
            player->hero_screen.y = player->py - world->camera.y - TILE_SIZE;
            
            // 清空
            player->moving = 0;
        }
    }
    if (world->map->logicmap.logic[player->tile_y][player->tile_x].exit_id == 1)
        {
            //找到传送点瓦片起始坐标
            int i = 0;
            while (player->tile_x != world->map->portals[i].from_x &&
                   player->tile_y != world->map->portals[i].from_y )
            {
               i++;
            }
            
            world->mapup = true;
            if (world->mapup == true)
        {
            //更新英雄瓦片坐标再更新地图
            player->tile_x = world->map->portals[i].to_x;  // 起始瓦片位置
            player->tile_y = world->map->portals[i].to_y;
            world->map = &(world->maps[world->map->portals[i].map1][world->map->portals[i].map2]);
            map_update(world,player);
            world->mapup = false;
        }
            return;
        }
        if(world->map->logicmap.logic[player->tile_y][player->tile_x].event_id == grassland)
        {
            player->battle_state = true;
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
