#ifndef WORLD_H
#define WORLD_H
#include <filesystem>
#include <cstdint>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
typedef enum {
    zero,
    battle,
    exit_map
} Event;

typedef struct {
    bool walkable;
    uint8_t exit_id;
    Event event_id;
    int tile_ID;
    SDL_Rect tile_rect;
} LogicTile;

struct Tileset {
    int firstgid;                   
    int tilecount;
    int columns;

    std::string name;      // tsj名字
    std::vector<LogicTile> tiles;   // local tiles
};//瓦片集


typedef struct {
    int from_x,from_y;
    int map1,map2;
    int to_x,to_y;
} Portal;

struct MapTileset  
{  
    int first_gid;  
    Tileset* ts;   
};

typedef struct {
    std::vector<std::vector<int>> map_bg;
    std::vector<std::vector<int>> map_mg;
    std::vector<std::vector<int>> logic_map;
    int map_w, map_h;
    Portal portals[10];
    std::vector<MapTileset> maptilesets;
    std::vector<LogicTile*> gid_lookup; //根据gid得出tmj里的id
    std::vector<std::string> name;  //tsj路径
} Maps;

class World{
    private:
        std::vector<Tileset> tilesets;  //瓦片集
        std::vector<std::filesystem::path> tileset_tsj_paths;   //瓦片集路径
    public:
        Maps *map;
        Maps maps[5][5];
        bool mapup;
        SDL_Rect camera;
        World();
        bool load_map(const std::string& filename);
        bool load_tileset(const std::filesystem::path& filename,int i);
        //bool load_tileset(const char* path);
        LogicTile get_tile(int id);

};
#endif 