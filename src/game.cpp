#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include "game.hpp"
#include "world.hpp"
#include "player.hpp"
#include "config.hpp"

void load_map(const char* path, Maps *map,SDL_Renderer *rdr);
bool load_logic_map_from_mask(const char* path, Maps* map);
void load_map_portal(const char* path, Maps* map);
void load_img(const char* path,SDL_Texture *img,SDL_Renderer *rdr);
void load_pokemon(const char* path, Pokemon *pokemon,SDL_Renderer *rdr);
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
    

    //创建背景地图
    load_map("asset/maps/0_0.png",&(world->maps[0][0]),game->rdr);
    //加载逻辑地图
    load_logic_map_from_mask("asset/maps/0_0_mask.png", &(world->maps[0][0]));
    //加载地图传送点
    load_map_portal("asset/portals/0_0.txt",&(world->maps[0][0]));

    load_map("asset/maps/0_1.png",&(world->maps[0][1]),game->rdr);
    load_logic_map_from_mask("asset/maps/0_1_mask.png", &(world->maps[0][1]));
    load_map_portal("asset/portals/0_1.txt",&(world->maps[0][1]));
    load_map("asset/maps/1_0.png",&(world->maps[1][0]),game->rdr);
    load_logic_map_from_mask("asset/maps/1_0_mask.png", &(world->maps[1][0]));
    load_map_portal("asset/portals/1_0.txt",&(world->maps[1][0]));
    //load_pokemon("asset/004b.png",game->battle.player,game->rdr);
    //load_pokemon("asset/007.png",game->battle.enemy,game->rdr);
    //game->battle.player->rect.x = 112;
    //一开始地图没变
    world->mapup = false;

    //加载战斗界面

    return 0;
}

void load_map(const char* path, Maps *map,SDL_Renderer *rdr)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        printf("Failed to load mask: %s\n", IMG_GetError());
        return ;
    }


    map->map_w = surf->w  ; 
    map->map_h = surf->h;

    //转换成纹理
    map->bg = SDL_CreateTextureFromSurface(rdr,surf);

     //检测
     if (!map->bg)
    {
        printf("CreateTexture Error: %s\n", SDL_GetError());
        getchar();
        return ;
    }
    SDL_FreeSurface(surf);
   
    return ;
}

bool load_logic_map_from_mask(const char* path, Maps* map)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        printf("Failed to load mask: %s\n", IMG_GetError());
        return false;
    }

    //转换为32位像素位
    surf = SDL_ConvertSurfaceFormat(surf,SDL_PIXELFORMAT_RGBA32,0);

    if (surf->w * TILE_SIZE != map->map_w || surf->h * TILE_SIZE != map->map_h) {
        printf("Mask size mismatch %s\n",path);
        SDL_FreeSurface(surf);
        return false;
    }

    SDL_LockSurface(surf);

    for (int y = 0; y < surf->h; y++) {
        for (int x = 0; x < surf->w; x++) {

            //像素的地址
            uint32_t pixel = *((uint32_t *)surf->pixels + y * (surf->pitch/4) + x);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, surf->format, &r, &g, &b);

            LogicTile* t = &map->logicmap.logic[y][x];

            // 默认值
            t->walkable = 1;
            t->exit_id = 0;
            t->event_id = road;
            if (r <= 20 && g <= 20 && b <= 20) {
                t->walkable = 0;              // 黑 墙
            }
            else if (r <= 20 && g >= 235 && b <= 20) {
                t->walkable = 1;                  //绿 路
            }
            else if (r >= 235 && g >= 235 && b >= 235) {
                t->exit_id = 1;               // 白 出口 
            }
            else if (r >= 235 && g <= 20 && b <= 20) {
                t->event_id = grassland;               // 红 草丛 
            }
        }
    }

    SDL_UnlockSurface(surf);
    SDL_FreeSurface(surf);
    return true;
}

void load_map_portal(const char* path, Maps* map)
{
    fflush(stdout);
    FILE *fp = fopen(path, "r");
    if (!fp) 
    {
    printf("Failed to open portal file: %s\n",path);
    return ;
    }
    int i=0;
    char line[256];
     while (fgets(line, sizeof(line), fp))
    {
fflush(stdout);
    // line 现在是一整行字符串
        if (line[0] == '#' || line[0] == '\n')
        continue;
        int n = sscanf(line,"%d %d %d %d %d %d",
            &map->portals[i].map1,
            &map->portals[i].map2,
            &map->portals[i].from_x,
            &map->portals[i].from_y,
            &map->portals[i].to_x,
            &map->portals[i].to_y);
        i++;
        if (n != 6) {
            printf("Bad portal line: %s", line);
            continue;
        }
        
    }
    fclose(fp);
    /*printf("%d %d %d %d %d %d\n",map->portals[0].map1,
            map->portals[0].map2,
            map->portals[0].from_x,
            map->portals[0].from_y,
            map->portals[0].to_x,
            map->portals[0].to_y);*/
    return ;
}

void load_pokemon(const char* path, Pokemon *pokemon,SDL_Renderer *rdr)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        printf("Failed to load mask: %s\n", IMG_GetError());
        return ;
    }
    //转换成纹理
    pokemon->b = SDL_CreateTextureFromSurface(rdr,surf);

     //检测
     if (!pokemon->b)
    {
        printf("CreateTexture Error: %s\n", SDL_GetError());
        getchar();
        return ;
    }
    SDL_FreeSurface(surf);
   
    return ;
}

void load_img(const char* path,SDL_Texture *img,SDL_Renderer *rdr)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        printf("Failed to load mask: %s\n", IMG_GetError());
        return ;
    }

    //转换成纹理
    img = SDL_CreateTextureFromSurface(rdr,surf);

     //检测
     if (!img)
    {
        printf("CreateTexture Error: %s\n", SDL_GetError());
        getchar();
        return ;
    }
    SDL_FreeSurface(surf);
   
    return ;
}


void Game_Quit(Game *game,World *world,Player *player)
{
    SDL_DestroyTexture(world->maps[0][0].bg);
    SDL_DestroyTexture(world->maps[0][1].bg);
    SDL_DestroyTexture(world->maps[1][0].bg);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(game->rdr);     //销毁渲染器
    SDL_DestroyWindow(game->window);   //销毁窗口
    SDL_Quit();                  //关闭程序
}