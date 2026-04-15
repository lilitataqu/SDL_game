#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define MAP_W 100
#define MAP_H 100
#define TILE_SIZE 32
#define TARGET_FPS   60
#define FRAME_TIME  (1000 / TARGET_FPS)
#define MOVE_FRAMES 8  // Frames per tile movement (Pokemon-style)

typedef enum{
    road,
    grassland
}Event;

typedef struct {
    uint8_t walkable;
    //地图的出入口
    uint8_t exit_id;
    //事件
    Event event_id;
} LogicTile;

typedef struct {
    LogicTile logic[MAP_H][MAP_W];
} LogicMap;

typedef struct {
    int from_x,from_y;
    int map1,map2;     // 目标地图 ID
    int to_x;
    int to_y;
} Portal;


typedef struct 
{
    SDL_Texture *bg;
    LogicMap logicmap;
    int map_w;
    int map_h;
    Portal portals[10];
}Maps;

typedef struct {
    Maps *map;
    Maps maps[5][5];
    bool mapup;
} World;

typedef enum {
    DIR_NONE = -1,
    DIR_UP = 0,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;


typedef struct {
    int tile_x;
    int tile_y;
    SDL_Texture *hero;
    int px,py;
    float x,y;
    float start_x, start_y;  // Starting position for interpolation
    int w,h;
    SDL_Rect hero_screen;
    int facing;
    int move_frames;         // Total frames for one tile movement
    int move_frame_count;    // Current frame in movement
    bool moving;
}SB;

typedef struct {
    char name[16];
    int power;     // 伤害值
} Skill;

typedef struct {
    char name[16];
    SDL_Texture *b;
    SDL_Rect rect;
    int max_hp;
    int hp;

    int attack;
    int defense;

    Skill skill;   // 当前版本：只允许 1 个技能
} Pokemon;

typedef enum {
    BATTLE_START,
    PLAYER_TURN,
    ENEMY_TURN,
    BATTLE_END
} BattleState;

typedef struct {
    Pokemon *player;
    Pokemon *enemy;
    SDL_Texture *bg;
    BattleState state;
} Battle;

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *rdr;

    SDL_Rect camera;

    World world;
    SB hero;

    SDL_Event event;

    Battle battle;

    SDL_Texture *canvas;

    SDL_Rect rect;
    
    int window_w;
    int window_h;

    int running ;
    int iu;

    int view_x,view_y;
    SDL_Texture *sid;
} Game;


int Game_Init(Game *game);
void Game_Quit(Game *game);
#endif