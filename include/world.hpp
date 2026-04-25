#ifndef WORLD_H
#define WORLD_H
#include <filesystem>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
//事件
typedef enum {
    zero,
    battle,
    exit_map
} Event;
//瓦片属性
typedef struct {
    bool walkable;
    uint8_t exit_id;
    Event event_id;
    int tile_ID;//瓦片集里的id
    int tiles_id;//瓦片集id
    SDL_Rect tile_rect;//瓦片集里的位置
} LogicTile;

struct Tileset {
    int firstgid;                   
    int tilecount;
    int columns;

    std::string name;      // tsj名字
    std::vector<LogicTile> tiles;   // local tiles
};//瓦片集


typedef struct {
    SDL_Rect rect;
    std::string target_map;
    int target_x,target_y;
    int direction;//朝向
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
    std::vector<Portal> portals;
    std::vector<MapTileset> maptilesets;
    std::vector<LogicTile*> gid_lookup; //根据gid得出tmj里的id
    std::vector<std::string> name;  //tsj路径
} Maps;

class World{
    private:
        std::vector<Tileset> tilesets;  //瓦片集
        std::vector<std::filesystem::path> tileset_tsj_paths;   //瓦片集路径
        std::vector<std::filesystem::path> maps_tmj_paths; //地图路径
    public:
        Maps *map;
        std::unordered_map<std::string, std::unique_ptr<Maps>> maps;
        bool mapup;
        SDL_Rect camera;
        World();
        bool load_map(const std::string& filename);
        bool load_tileset(const std::filesystem::path& filename,int i);
        //bool load_tileset(const char* path);
        LogicTile get_tile(int id);

};
#endif 