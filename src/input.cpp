#include "game.hpp"
#include "tex_manager.hpp"

void map_update(Game *game)
{
     //小数，计算时用到的世界坐标
    game->hero.x = game->hero.tile_x * TILE_SIZE;
    game->hero.y = game->hero.tile_y * TILE_SIZE;
    //int 屏幕坐标
    game->hero.px = game->hero.tile_x * TILE_SIZE;
    game->hero.py =  game->hero.tile_y * TILE_SIZE;
    //帧坐标
    game->hero.start_x = game->hero.x;
    game->hero.start_y = game->hero.y;
    game->hero.move_frame_count = 0;
    game->hero.moving = false;

    //摄像机左上角在世界中的位置 摄像机大小 = 窗口大小
    game->camera.x = 0 ,game->camera.y = 0,
    //人物渲染的起始坐标，和px一样
    game->hero.hero_screen.x = game->hero.px - game->camera.x;
    game->hero.hero_screen.y = game->hero.py - game->camera.y - TILE_SIZE;

    game->hero.moving = 0;
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


void player_update(Game *game,Tex_Manager *tex)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    Direction dir = get_dir_from_key(keys);

    // 永远允许更新朝向
    if (dir != DIR_NONE)
        game->hero.facing = dir;

    // 只有不在移动中，才尝试移动
    if (!game->hero.moving && dir != DIR_NONE) {
        int dx, dy;
        dir_to_offset(dir, &dx, &dy);

        int nx = game->hero.tile_x + dx;
        int ny = game->hero.tile_y + dy;

        if (game->world.map->logicmap.logic[ny][nx].walkable) {
            game->hero.moving = 1;
            
            // Store starting position for interpolation
            game->hero.start_x = game->hero.x;
            game->hero.start_y = game->hero.y;
            
            // Reset frame counter
            game->hero.move_frame_count = 0;

            game->hero.tile_x = nx;
            game->hero.tile_y = ny;

            game->hero.px = nx * TILE_SIZE;
            game->hero.py = ny * TILE_SIZE;
        }
    }

    // 如果正在移动，执行插值
    if (game->hero.moving) 
    {
        // 第一帧
        game->hero.move_frame_count++;
        
        // 8帧间的差距
        float progress = (float)game->hero.move_frame_count / (float)game->hero.move_frames;
        
        
        game->hero.x = game->hero.start_x + (game->hero.px - game->hero.start_x) * progress;
        game->hero.y = game->hero.start_y + (game->hero.py - game->hero.start_y) * progress;
        // 摄像头跟随
        game->camera.x = (int)(game->hero.x - game->view_x);
        game->camera.y = (int)(game->hero.y - game->view_y);
        
        // 摄像头范围
        if (game->camera.x < 0)
            game->camera.x = 0;
        if (game->camera.x > game->world.map->map_w - game->camera.w)
            game->camera.x = game->world.map->map_w - game->camera.w;
        if (game->camera.y < 0)
            game->camera.y = 0;
        if (game->camera.y > game->world.map->map_h - game->camera.h)
            game->camera.y = game->world.map->map_h - game->camera.h;

        // Update hero screen position for rendering
        game->hero.hero_screen.x = (int)(game->hero.x - game->camera.x);
        game->hero.hero_screen.y = (int)(game->hero.y - game->camera.y) - TILE_SIZE;
        // Check if movement is complete
        if (game->hero.move_frame_count >= game->hero.move_frames)
        {
            // 将屏幕坐标更新到世界坐标
            game->hero.x = game->hero.px;
            game->hero.y = game->hero.py;
            
            
            // 移动摄像头
            game->camera.x = (game->hero.px - game->view_x);
            game->camera.y = (game->hero.py - game->view_y);
            
            if (game->camera.x < 0)
                game->camera.x = 0;
            if (game->camera.x > game->world.map->map_w - game->camera.w)
                game->camera.x = game->world.map->map_w - game->camera.w;
            if (game->camera.y < 0)
                game->camera.y = 0;
            if (game->camera.y > game->world.map->map_h - game->camera.h)
                game->camera.y = game->world.map->map_h - game->camera.h;
            
            game->hero.hero_screen.x = game->hero.px - game->camera.x;
            game->hero.hero_screen.y = game->hero.py - game->camera.y - TILE_SIZE;
            
            // 清空
            game->hero.moving = 0;
        }
    }
    if (game->world.map->logicmap.logic[game->hero.tile_y][game->hero.tile_x].exit_id == 1)
        {
            //找到传送点瓦片起始坐标
            int i = 0;
            while (game->hero.tile_x != game->world.map->portals[i].from_x &&
                   game->hero.tile_y != game->world.map->portals[i].from_y )
            {
               i++;
            }
            
            game->world.mapup = true;
            if (game->world.mapup == true)
        {
            //更新英雄瓦片坐标再更新地图
            game->hero.tile_x = game->world.map->portals[i].to_x;  // 起始瓦片位置
            game->hero.tile_y = game->world.map->portals[i].to_y;
            game->world.map = &(game->world.maps[game->world.map->portals[i].map1][game->world.map->portals[i].map2]);
            map_update(game);
            game->world.mapup = false;
        }
            return;
        }
        if(game->world.map->logicmap.logic[game->hero.tile_y][game->hero.tile_x].event_id == grassland)
        {
            game->iu = 0;
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
