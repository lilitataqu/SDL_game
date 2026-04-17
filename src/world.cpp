#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
#include "world.hpp"
#include "tinyxml2.h"
using namespace tinyxml2;
#include "config.hpp"
World::World()
{
    tiles[0].tile_rect.x = 0 ;
    tiles[0].tile_rect.y = 0 ;
    tiles[0].tile_rect.w = 32 ;
    tiles[0].tile_rect.h = 32 ;
    if(!(load_map("asset/test.tmj")))
    {
        printf("asset\test.tmj load faile \n");
    }
    if(!(load_tileset("asset/tiles.tsx")))
    {
        printf(" asset\tiles.tsx load faile \n");
    }
}
bool World::load_map(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    json j;
    file >> j;

    int width  = j["width"];
    int height = j["height"];

    auto data = j["layers"][0]["data"].get<std::vector<int>>();

    Maps& current = maps[0][0];
    current.map_w = width;
    current.map_h = height;
    current.logicmap.resize(height, std::vector<uint8_t>(width, 0));
    for (size_t i = 0; i < data.size(); ++i)
    {
        int x = i % width;
        int y = i / width;
        int tile = data[i] - 1; // Tiled偏移
        if (tile < 0) tile = 0;  

        current.logicmap[y][x] = static_cast<uint8_t>(tile);
    }

    return true;
}

bool World::load_tileset(const char* path)
{
    XMLDocument doc;
    //加载文件
    if (doc.LoadFile(path) != XML_SUCCESS)
    {  
        printf("load tsx failed\n");
        return false;
    }

    XMLElement* root = doc.FirstChildElement("tileset");
    //XMLElement* tex_rect = root->FirstChildElement("image");
    int columns = root->IntAttribute("columns");
    int tilecount = root->IntAttribute("tilecount");
    for (int id = 0; id < tilecount; id++)
    {
        tiles[id].tile_rect.x = (id % columns) * TILE_SIZE;
        tiles[id].tile_rect.y = (id / columns) * TILE_SIZE;
        tiles[id].tile_rect.w = TILE_SIZE;
        tiles[id].tile_rect.h = TILE_SIZE;
    }
    for (XMLElement* tile = root->FirstChildElement("tile");
         tile != NULL;
         tile = tile->NextSiblingElement("tile"))
    {
        int id = tile->IntAttribute("id");
        XMLElement* props = tile->FirstChildElement("properties");
        if (!props) continue;

        for (XMLElement* prop = props->FirstChildElement("property");
             prop != NULL;
             prop = prop->NextSiblingElement("property"))
        {   //Attribute返回指定属性名的值
            const char* name = prop->Attribute("name");
            //strcmp是个比较字符串是否相等的c++标准库
            if (strcmp(name, "walkable") == 0)
            {
                tiles[id].walkable = prop->BoolAttribute("value");
            }
            else{
                printf("%s value error\n",path);
            }
        }
        
    }
    return true;
}
LogicTile World::get_tile(const uint8_t id)
{
    if (id >= tile_num) return tiles[0];//越界时
    return tiles[id];
}