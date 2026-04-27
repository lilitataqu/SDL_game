#include <SDL2/SDL_image.h>
#include "tex_manager.hpp"
#include "config.hpp"

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
        "asset/png/内部1.png",
        "asset/png/外部春天1.png"
        
        
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
        "asset/ui/menu.png",
        "asset/ui/menu选中框.png"
    };

    //渲染瓦片集
    for(int i=0 ; i<tileset_png_paths.size();i++){
        tiles.push_back(IMG_LoadTexture(renderer,tileset_png_paths[i].c_str()));
        if(tiles[i] == nullptr)
        {
            printf("Texture load failed: %s\nSDL Error: %s\n", tileset_png_paths[0].c_str(), SDL_GetError());
        }
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
    //渲染ui
    for(int i = 0; i < UI_NUM; i++)
    {
        SDL_Surface *face = IMG_Load(ui_paths[i].c_str());
        if(face == nullptr)
        {
            printf("Texture load failed: %s\n", ui_paths[i].c_str());
            continue;
        }
        ui[i].tex = SDL_CreateTextureFromSurface(renderer, face);
        
        if(ui[i].tex ==  nullptr)
        {
            printf("Texture load failed: %s\n", ui_paths[i].c_str());
        }
        //初始化
        ui[i].rect.w = face->w;
        ui[i].rect.h =face->h;
        ui[i].able = true;
        SDL_FreeSurface(face);
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
    ui[0].rect.x = 500;
    ui[0].rect.y = 20;
    ui[1].rect.x = ui[0].rect.x + 17;
    ui[1].rect.y = ui[0].rect.y + 21;
}

void Tex_Manager::move_mens_box()
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    uint32_t now = SDL_GetTicks();
    if(ui[MENU_SELECT_BOX].rect.y >= 79 + ui[0].rect.y && (keys[SDL_SCANCODE_UP ]|| keys[SDL_SCANCODE_W]))
    {
        if (menu_box.firstPress)
        {
            ui[1].rect.y -= 58;  // 第一次立即触发
            menu_box.firstPress = false;
            menu_box.lastTime = now;
        } 
        else
        {
            if (now - menu_box.lastTime > 200) {
                ui[1].rect.y -= 58;
                menu_box.lastTime = now - (80);
            }
        }
        return;
    }
    else if (ui[MENU_SELECT_BOX].rect.y <= 21+58*5+ui[0].rect.y && (keys[SDL_SCANCODE_DOWN ]|| keys[SDL_SCANCODE_S]))
    {
        if (menu_box.firstPress)
        {
            ui[1].rect.y += 58;  // 第一次立即触发
            menu_box.firstPress = false;
            menu_box.lastTime = now;
        } 
        else
        {
            if (now - menu_box.lastTime > 200) {
                ui[1].rect.y += 58;
                menu_box.lastTime = now - (80);
            }
        }
        return;
    }
    else{
        menu_box.firstPress = true;
    }
    return;
    
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