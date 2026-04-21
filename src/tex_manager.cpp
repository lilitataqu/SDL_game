#include <SDL2/SDL_image.h>
#include "tex_manager.hpp"

Tex_Manager::Tex_Manager(SDL_Renderer* renderer)
{
    // 初始化 SDL2_image PNG 支持
    IMG_Init(IMG_INIT_PNG);

    //渲染主角
    SDL_Surface *player_face = IMG_Load("asset/hero.png");

    if (player_face== NULL)            
    {
        printf("IMG_Load: %s\n", SDL_GetError());
        getchar();
        SDL_Quit();
        return ;
    }

    //给主角接受，
    player = SDL_CreateTextureFromSurface(renderer, player_face);
    //释放临时表面
    SDL_FreeSurface(player_face);

    //检测
     if (!player)
    {
        printf("CreateTexture Error: %s\n", SDL_GetError());
        getchar();
        return ;
    }

    //瓦片集
    tileset_png_paths = {
        "asset/png/1_1.png"
    };
    //战斗背景
    bg_paths = {
        "asset/battlegrass.png"
    };
    //宝可梦
    pokemon_paths = {
        "asset/004b.png",
        "asset/007.png"
    };
    //ui
    ui_paths = {
        "asset/cloth.png"
    };

    //渲染瓦片集
    tiles = IMG_LoadTexture(renderer,tileset_png_paths[0].c_str());
    if(tiles == nullptr)
    {
        printf("Texture load failed: %s\n", tileset_png_paths[0].c_str());
    }
    //渲染战斗背景
    for(int i = 0; i < BATTLE_BG_NUM; i++)
    {
        btl_bg[i].tex = IMG_LoadTexture(renderer, bg_paths[i].c_str());

        if(btl_bg[i].tex ==  nullptr)
        {
            printf("Texture load failed: %s\n", bg_paths[i].c_str());
        }
    }
    //渲染战斗宝可梦
    for(int i = 0; i < BATTLE_POKEMON_NUM; i++)
    {
        pokemon_tex[i].tex = IMG_LoadTexture(renderer, pokemon_paths[i].c_str());

        if(pokemon_tex[i].tex ==  nullptr)
        {
            printf("Texture load failed: %s\n", pokemon_paths[i].c_str());
        }
    }
    //渲染战斗ui
    for(int i = 0; i < UI_NUM; i++)
    {
        ui[i].tex = IMG_LoadTexture(renderer, ui_paths[i].c_str());

        if(ui[i].tex ==  nullptr)
        {
            printf("Texture load failed: %s\n", ui_paths[i].c_str());
        }
    }
    //初始化几个矩形
    pokemon_tex[0].rect.x = 32;
    pokemon_tex[0].rect.y = 64;
    pokemon_tex[0].rect.w = 64;
    pokemon_tex[0].rect.h = 64;
    pokemon_tex[1].rect.x = 144;
    pokemon_tex[1].rect.y = 16;
    pokemon_tex[1].rect.w = 64;
    pokemon_tex[1].rect.h = 64;
    ui[0].rect.x = 0;
    ui[0].rect.y = 112;
    ui[0].rect.w = 240;
    ui[0].rect.h = 48;
}

Tex_Manager::~Tex_Manager()
{
    //销毁玩家
    SDL_DestroyTexture(player);

    //销毁战斗背景
    for(int i = 0; i < BATTLE_BG_NUM; i++)
    {
        if(btl_bg[i].tex)
        {
            SDL_DestroyTexture(btl_bg[i].tex);
            btl_bg[i].tex =  nullptr;
        }
    }
    //销毁宝可梦
    for(int i = 0; i < BATTLE_POKEMON_NUM; i++)
    {
        if(pokemon_tex[i].tex)
        {
            SDL_DestroyTexture(pokemon_tex[i].tex);
            pokemon_tex[i].tex =  nullptr;
        }
    }
    //销毁ui
    for(int i = 0; i < UI_NUM ; i++)
    {
        if(ui[i].tex)
        {
            SDL_DestroyTexture(ui[i].tex);
            ui[i].tex =  nullptr;
        }
    }
}