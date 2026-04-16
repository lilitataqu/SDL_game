#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "game.hpp"
#include "render.hpp"
#include "input.hpp"
#include "tex_manager.hpp"
#include "render.hpp"
#include "config.hpp"
int main(int argc, char *argv[])
{
    Game game;
    Player player;
    World world;
    //创建窗口 渲染器 引入 人物 地图
    if (Game_Init(&game,&world,&player) != 0) {
    Game_Quit(&game,&world,&player);
    return 1;
}
    
    Render_Init(&game,&world,&player);
    
    game.running = 1;
    game.iu = 0 ;

    //TEXS tex;
    //加载渲染纹理
    //TextureManager_Init(game.rdr,&tex);
    Tex_Manager tex(game.rdr);

    //帧数与间隔时间
    Uint32 frame_start = SDL_GetTicks();
    
    while (game.running)
    {
        //起始帧
        frame_start = SDL_GetTicks();

        ui(&game);

         // 如果收到退出事件，立即跳出循环
        if (!game.running) {
            break;
        }
        
        //player_update(&game);
        
        draw(&game,&tex,&world,&player);

        //刷新屏幕，并等待一个帧
        Uint32 frame_cost = SDL_GetTicks() - frame_start;
        if (frame_cost < FRAME_TIME) 
        {
            SDL_Delay(FRAME_TIME - frame_cost);
        }
    }

    Game_Quit(&game,&world,&player);
    
    return 0;
}

