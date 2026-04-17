#ifndef WORLD_H
#define WORLD_H

#include <cstdint>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
typedef enum {
    zero,
    battle,
    exit_map
} Event;

typedef enum{
    road,
    wall,
    tile_num
}Tile_ID;

typedef struct {
    bool walkable;
    uint8_t exit_id;
    Event event_id;
    uint8_t tile_ID;
    SDL_Rect tile_rect;
} LogicTile;


typedef struct {
    int from_x,from_y;
    int map1,map2;
    int to_x,to_y;
} Portal;

typedef struct {
    std::vector<std::vector<uint8_t>> logicmap;
    int map_w, map_h;
    Portal portals[10];
} Maps;

class World{
    private:
        LogicTile tiles[tile_num];
    public:
        Maps *map;
        Maps maps[5][5];
        bool mapup;
        SDL_Rect camera;
        World();
        bool load_map(const std::string& filename);
        bool load_tileset(const char* path);
        LogicTile get_tile(const uint8_t id);

};
#endif 