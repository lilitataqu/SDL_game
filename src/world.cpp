#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
#include "world.hpp"
#include "tinyxml2.h"
using namespace tinyxml2;
#include "config.hpp"
World::World()
{
    if(!(load_map("asset/maps/0_0.tmj")))
    {
        printf("asset/maps/0_0.tmj load faile \n");
    }
    if(!(load_tileset("asset/png/tiles2.tsj")))
    {
        printf(" asset/png/tiles2.tsj load faile \n");
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
    current.logicmap.resize(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < data.size(); ++i)
    {
        int x = i % width;
        int y = i / width;
        int tile = data[i] - 1; // Tiled偏移
        if (tile < 0) tile = 0;  
        current.logicmap[y][x] = tile;
    }

    return true;
}
//加载瓦片集
bool World::load_tileset(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    json j;
    file >> j;
    int columns = j["columns"];
    int tilecount = j["tilecount"];
    tiles.resize(tilecount);
    for(int id=0 ; id < tilecount ; id++)
    {
        tiles[id].tile_rect.x = (id % columns) * TILE_SIZE;
        tiles[id].tile_rect.y = (id / columns) * TILE_SIZE;
        tiles[id].tile_rect.w = TILE_SIZE;
        tiles[id].tile_rect.h = TILE_SIZE;
    }
    /*等价的for循环
    auto& tile_s = j["tiles"];
    for (int i = 0; i < tile_s.size(); i++)
    {
        auto& tile = tiles[i];
    }
    */
    for (auto& tile : j["tiles"])
    {
        int id = tile["id"];
        tiles[id].tile_ID = id;
        
        if (tile.contains("properties"))
        {
            for (auto& prop : tile["properties"])
            {
                std::string name = prop["name"];
                if (name == "walkable")
                {
                    tiles[id].walkable = prop["value"];
                }
            }
        }
    }
    return true;
}
/*
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
    if(root == nullptr) return false;
    int columns = root->IntAttribute("columns");
    int tilecount = root->IntAttribute("tilecount");
    tiles.resize(tilecount);
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
        if(tile == nullptr) return false;
        int id = tile->IntAttribute("id");
        XMLElement* props = tile->FirstChildElement("properties");
        if (!props) return false;

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
*/
LogicTile World::get_tile(const uint8_t id)
{
    if (id >= tiles.size()) return tiles[0];//越界时
    return tiles[id];
}