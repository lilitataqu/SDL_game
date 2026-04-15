#include "game.hpp"
#include "input.hpp"
#include "tex_manager.hpp"
#include "ttf_manager.hpp"
void Render_Init(Game *game)
{
    //出生的地图
    game->world.map = &(game->world.maps[0][0]);
    // 初始化英雄位置和属性
    game->hero.w = 32;
    game->hero.h = 64;
    game->hero.tile_x = 12;  // 起始瓦片位置
    game->hero.tile_y = 9;
     //小数，计算时用到的世界坐标
    game->hero.x = game->hero.tile_x * TILE_SIZE;
    game->hero.y = game->hero.tile_y * TILE_SIZE;
    //int 屏幕坐标
    game->hero.px = game->hero.tile_x * TILE_SIZE;
    game->hero.py =  game->hero.tile_y * TILE_SIZE;
    //帧坐标
    game->hero.start_x = game->hero.x;
    game->hero.start_y = game->hero.y;

    game->hero.move_frames = MOVE_FRAMES;
    game->hero.move_frame_count = 0;
    game->hero.moving = false;
    game->hero.facing = DIR_DOWN;

     //摄像机左上角在世界中的位置 摄像机大小 = 窗口大小
    game->camera.x = 0 ,game->camera.y = 0,
    game->camera.w = game->window_w,game->camera.h = game->window_h;
    //视图距离
    game->view_x = (game->camera.w - game->hero.w)/2;
    game->view_y = (game->camera.h - game->hero.h)/2;
    //摄像头坐标
    game->camera.x = game->hero.px - game->view_x;
    game->camera.y = game->hero.py - game->view_y;
    //人物在屏幕的大小
    game->hero.hero_screen.w =  game->hero.w;
    game->hero.hero_screen.h =  game->hero.h;
    //人物渲染的起始坐标，和px一样
    game->hero.hero_screen.x = game->hero.px - game->camera.x;
    game->hero.hero_screen.y = game->hero.py - game->camera.y - TILE_SIZE;

    game->hero.moving = 0;

    //画布
    game->rect.w = game->window_w,game->rect.h = game->window_h;
    game->rect.x = 0 ,game->rect.y = 0 ;

    //ttf 傻逼
    font = TTF_OpenFont("asset/Huayuan Gothic.ttf", 24);

    if(!font)
    {
        printf("Font load error: %s\n", TTF_GetError());
    }
    game->sid = CreateTextTexture( game->rdr,
                                font,
                               "河南",
                                {255,255,255,255});
    /*
    //试着设置传送点
    game->world.maps[0][0].portals[0].map1 = 0;
    game->world.maps[0][0].portals[0].map2 = 1;
    game->world.maps[0][0].portals[0].to_x = 14;
    game->world.maps[0][0].portals[0].to_y = 13;
    game->world.maps[0][0].portals[0].from_x = 16;
    game->world.maps[0][0].portals[0].from_y = 15;

    game->world.maps[0][1].portals[0].map1 = 0;
    game->world.maps[0][1].portals[0].map2 = 0;
    game->world.maps[0][1].portals[0].to_x = 16;
    game->world.maps[0][1].portals[0].to_y = 15;
    game->world.maps[0][1].portals[0].from_x = 14;
    game->world.maps[0][1].portals[0].from_y = 13;
    */
}

void draw_map(Game *game)
{
   
    // 渲染纹理（透明自动生效）
    SDL_RenderCopy(game->rdr, game->world.map->bg, &(game->camera), NULL);
    SDL_RenderCopy(game->rdr, game->hero.hero, NULL,&(game->hero.hero_screen));

}

void draw_bg(Game *game,Tex_Manager *tex)
{

    // 渲染纹理（透明自动生效）
    for(int i = 0 ; i < BATTLE_BG_NUM ; i++)
    {
        
        if(tex->btl_bg[i].able == 1 && tex->btl_bg[i].tex != NULL)
        {
        SDL_RenderCopy(game->rdr, tex->btl_bg[i].tex,NULL,NULL);
        }

    }
}

void draw_pokemon_btl(Game *game , Tex_Manager *tex)
{
    // 渲染纹理（透明自动生效）
    for(int i = 0 ; i < BATTLE_POKEMON_NUM ; i++)
    {
        
        if(tex->pokemon_tex[i].able == 1 && tex->pokemon_tex[i].tex != NULL)
        {
        SDL_RenderCopy(game->rdr, tex->pokemon_tex[i].tex,NULL,&(tex->pokemon_tex[i].rect));
        }

    }
}

void draw_ui(Game *game , Tex_Manager *tex)
{
    // 渲染纹理（透明自动生效）
    for(int i = 0 ; i < UI_NUM ; i++)
    {
        
        if(tex->ui[i].able == 1 )
        {
        SDL_RenderCopy(game->rdr, tex->ui[i].tex,NULL,&(tex->ui[i].rect));
        }

    }
    SDL_RenderCopy(game->rdr,game->sid,NULL,NULL);
}

void draw(Game *game , Tex_Manager *tex)
{
    //切换渲染目标
    SDL_SetRenderTarget(game->rdr,game->canvas);
    //清空屏幕
    SDL_RenderClear(game->rdr);
    if(game->iu == 1)
    {
        player_update(game,tex);
        draw_map(game);  
    }
    if(game->iu == 0)
    {
        draw_bg(game,tex);
        draw_pokemon_btl(game,tex);
    }
    draw_ui(game,tex);
    SDL_SetRenderTarget(game->rdr,NULL);
    SDL_RenderClear(game->rdr);
    SDL_RenderCopy(game->rdr,game->canvas,NULL,&(game->rect));
    //渲染呈现
    SDL_RenderPresent(game->rdr);
}