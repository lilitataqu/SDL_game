#include "world.hpp"
#include "game.hpp"
#include "input.hpp"
#include "tex_manager.hpp"
#include "player.hpp"
#include "ttf_manager.hpp"
#include "config.hpp"
void Render_Init(Game *game,World *world,Player *player)
{
    

    //出生的地图
    world->map = &(world->maps[0][0]);
    //摄像机左上角在世界中的位置 摄像机大小 = 窗口大小
    world->camera.x = 0 ,world->camera.y = 0,
    world->camera.w = game->window_w,world->camera.h = game->window_h;
    //视图距离
    game->view_x = (world->camera.w - player->w)/2;
    game->view_y = (world->camera.h - player->h)/2;
    //摄像头坐标
    world->camera.x = player->px - game->view_x;
    world->camera.y = player->py - game->view_y;
   
    //人物渲染的起始坐标，和px一样
    player->hero_screen.x = player->px - world->camera.x;
    player->hero_screen.y = player->py - world->camera.y - TILE_SIZE;

    player->moving = 0;

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
}

void draw_map(Game *game,World *world,Player *player,Tex_Manager *tex)
{
   
    // 渲染纹理（透明自动生效）
    SDL_RenderCopy(game->rdr, world->map->bg, &(world->camera), NULL);
    SDL_RenderCopy(game->rdr, tex->player, NULL,&(player->hero_screen));

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

void draw(Game *game , Tex_Manager *tex,World *world,Player *player)
{
    //切换渲染目标
    SDL_SetRenderTarget(game->rdr,game->canvas);
    //清空屏幕
    SDL_RenderClear(game->rdr);
    if(game->iu == 0)
    {
        player->player_update(world , player , tex);
        draw_map(game,world,player,tex);  
    }
    if(game->iu == 1)
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