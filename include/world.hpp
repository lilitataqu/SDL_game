#ifndef WORLD_H
#define WORLD_H

#include <cstdint>
#include <SDL2/SDL.h>
typedef enum {
    road,
    grassland
} Event;

typedef struct {
    uint8_t walkable;
    uint8_t exit_id;
    Event event_id;
} LogicTile;

typedef struct {
    LogicTile logic[100][100];
} LogicMap;

typedef struct {
    int from_x,from_y;
    int map1,map2;
    int to_x,to_y;
} Portal;

typedef struct {
    SDL_Texture *bg;
    LogicMap logicmap;
    int map_w, map_h;
    Portal portals[10];
} Maps;

typedef struct {
    Maps *map;
    Maps maps[5][5];
    bool mapup;
    SDL_Rect camera;
} World;
#endif 