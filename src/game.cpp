#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include "game.hpp"
#include "world.hpp"
#include "player.hpp"
#include "config.hpp"

int Game_Init(Game *game,World *world,Player *player)
{
    //全部值0或空
    memset(game, 0, sizeof(Game));
     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "SDL_Init failed",
            SDL_GetError(),
            NULL
        );
        return 0;
    }

    
    game->window_w = WINDOW_W;
    game->window_h = WINDOW_H;
    game->running = 1;
   
    game->window = SDL_CreateWindow(
            "存在与虚无",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            game->window_w,
            game->window_h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
            
        );//flags是标志数 创建窗口

        //创建窗口是否失败
    if (game->window == NULL)            
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        getchar();
        return -1;
    }

    //创建渲染器
    game->rdr = SDL_CreateRenderer(game->window,0,SDL_RENDERER_ACCELERATED );
    //渲染器创建成功没
    if (game->rdr == NULL)            
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        getchar();
        return -1;
    }

    //设置渲染颜色
    SDL_SetRenderDrawColor(game->rdr,0,0,0,0);
    //清除屏幕
    SDL_RenderClear(game->rdr);

    //初始化ttf
    if(TTF_Init() == -1)
    {
        printf("TTF_Init Error: %s\n", TTF_GetError());
    }

    //创建画布
    game->canvas = SDL_CreateTexture(
        game->rdr,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,  // ⭐ 关键
        game->window_w, game->window_h
        );
    if(SDL_SetRenderTarget(game->rdr, game->canvas) != 0)
    {
        printf("SetRenderTarget error: %s\n", SDL_GetError());
    }
      
    //请输入文本
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "IMG_Init failed",
        IMG_GetError(),
        NULL
    );
    return -1;
    }
    world->mapup = false;
    game->state = State::WALK;
    
    return 0;
}

void Game_Quit(Game *game,World *world,Player *player)
{
    /*
    SDL_DestroyTexture(world->maps[0][0].bg);
    SDL_DestroyTexture(world->maps[0][1].bg);
    SDL_DestroyTexture(world->maps[1][0].bg);*/
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(game->rdr);     //销毁渲染器
    SDL_DestroyWindow(game->window);   //销毁窗口
    SDL_Quit();                  //关闭程序
}