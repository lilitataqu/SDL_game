#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "world.hpp"
#include "player.hpp"
//struct World;
//class Player;
enum class State{
    WALK,
    UI,
    BATTLE
};


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

    SDL_Event event;

    State state;

    Battle battle;

    SDL_Texture *canvas;

    SDL_Rect rect;
    
    
    int window_w;
    int window_h;

    int running ;

    int view_x,view_y;
    SDL_Texture *sid;
} Game;


int Game_Init(Game *game,World *world,Player *player);
void Game_Quit(Game *game,World *world,Player *player);
#endif