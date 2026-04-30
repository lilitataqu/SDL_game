#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "render.hpp"
#include "input.hpp"
#include "tex_manager.hpp"
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
    player.battle_state = false;

    //TEXS tex;
    //加载渲染纹理
    //TextureManager_Init(game.rdr,&tex);
    Tex_Manager tex(game.rdr);
    //请开始你的堆栈，栈处理键盘输入
    StateManager stateMgr;
    //帧数与间隔时间
    Uint32 frame_start = SDL_GetTicks();
    
    while (game.running)
    {
        //起始帧
        frame_start = SDL_GetTicks();
        //切换渲染目标
        SDL_SetRenderTarget(game.rdr,game.canvas);
        //清空屏幕
        SDL_RenderClear(game.rdr);
        
        //先输入，画面更新
        input(&game,&stateMgr,&tex,&world,&player);

         // 如果收到退出事件，立即跳出循环
        if (!game.running) {
            break;
        }
        //渲染
        SDL_SetRenderTarget(game.rdr,NULL);
        SDL_RenderCopy(game.rdr,game.canvas,NULL,&(game.rect));
        //渲染呈现
        SDL_RenderPresent(game.rdr);
        

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

