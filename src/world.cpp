#include <fstream>
#include <iostream>
#include <filesystem>
#include "json.hpp"
using json = nlohmann::json;
#include "world.hpp"
#include "tinyxml2.h"
using namespace tinyxml2;
#include "config.hpp"
World::World()
{
    tileset_tsj_paths = {
        "asset/png/1_1.tsj"
    };
    int i=0;
    //这里写多少个瓦片集
    tilesets.resize(1);
    for (const auto& p : tileset_tsj_paths)
    {
        if(!(load_tileset(p,i)))
        {
            std::cout << p << "load failed" << std::endl;
        }
        i++;
    }

    if(!(load_map("asset/maps/0_0.tmj")))
    {
        printf("asset/maps/0_0.tmj load faile \n");
    }
    
}
bool World::load_map(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    json j;
    file >> j;

    int width  = j["width"];
    int height = j["height"];

    auto data_bg = j["layers"][0]["data"].get<std::vector<int>>();
    auto data_mg = j["layers"][1]["data"].get<std::vector<int>>();

    Maps& current = maps[0][0];
    //寻找最大gid
    int max_gid = 0; 
    //遍历tilesets
    int fid = 0;
    for (auto& ts : j["tilesets"])
    {
        //根据路径得到tsj的名字
        std::string tsj_path = ts["source"].get<std::string>();
        std::filesystem::path p(tsj_path);
        current.name.push_back(p.stem().string());
        
        // 在全局池里找瓦片集
        for (auto& t : tilesets)
        {
            if (current.name[fid] == t.name )
            {
                current.maptilesets.push_back(MapTileset{ts["firstgid"],&t});//显示构造，vscode大战c++
                break;
            }
        }
        if(static_cast<int>(current.maptilesets.size()) - 1 != fid) return false;
        //maptilesets没初始化就可能崩掉
        int _gid = current.maptilesets[fid].first_gid + current.maptilesets[fid].ts->tilecount -1 ;
        if(max_gid < _gid) max_gid = _gid;
        fid++;
    }
    int a = current.maptilesets.size() - 1;//遍历maptilesets
    printf("%d",max_gid);
    //申请空间
    current.gid_lookup.resize(max_gid);
    for(int i=max_gid ; i>=1 ; i--){
        int id = current.maptilesets[a].first_gid;
        if(i < id) a--;
        current.gid_lookup[i-1] = &(current.maptilesets[a].ts->tiles[i - id]);
    }
    //初始化背景 中景 逻辑地图
    current.map_w = width;
    current.map_h = height;
    current.map_bg.resize(height, std::vector<int>(width, 0));
    current.map_mg.resize(height, std::vector<int>(width, 0));
    current.logic_map.resize(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < data_bg.size(); ++i)
    {
        int x = i % width;
        int y = i / width;
        //背景
        int tile_bg = data_bg[i] - 1; // Tiled偏移
        if (tile_bg < 0) tile_bg = 0;  
        current.map_bg[y][x] = tile_bg;
        //中景 data数组为0，表示此地无银300两
        int tile_mg = data_mg[i] - 1; // Tiled偏移
        //if (tile_mg < 0) tile_mg = 0;  

        current.map_mg[y][x] = tile_mg;
        current.logic_map[y][x] = tile_mg > tile_bg ? tile_mg : tile_bg; 
    }
    //printf("%d %d %d\n",current.map_mg[31][11],current.map_mg[32][11],current.map_mg[33][11]);
    return true;
}
//加载瓦片集
bool World::load_tileset(const std::filesystem::path& filename,int i)
{
    
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    json j;
    file >> j;
    tilesets[i].name = j["name"];
    tilesets[i].columns = j["columns"];
    tilesets[i].tilecount = j["tilecount"];
    tilesets[i].tiles.resize(tilesets[i].tilecount);
    
    for(int id=0 ; id < tilesets[i].tilecount ; id++)
    {
        tilesets[i].tiles[id].tile_rect.x = (id % tilesets[i].columns) * TILE_SIZE;
        tilesets[i].tiles[id].tile_rect.y = (id / tilesets[i].columns) * TILE_SIZE;
        tilesets[i].tiles[id].tile_rect.w = TILE_SIZE;
        tilesets[i].tiles[id].tile_rect.h = TILE_SIZE;
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
        tilesets[i].tiles[id].tile_ID = id;
        
        if (tile.contains("properties"))
        {
            for (auto& prop : tile["properties"])
            {
                std::string name = prop["name"];
                if (name == "walkable")
                {
                    tilesets[i].tiles[id].walkable = prop["value"];
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
        tilesets[i].tiles[id].tile_rect.x = (id % columns) * TILE_SIZE;
        tilesets[i].tiles[id].tile_rect.y = (id / columns) * TILE_SIZE;
        tilesets[i].tiles[id].tile_rect.w = TILE_SIZE;
        tilesets[i].tiles[id].tile_rect.h = TILE_SIZE;
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
                tilesets[i].tiles[id].walkable = prop->BoolAttribute("value");
            }
            else{
                printf("%s value error\n",path);
            }
        }
        
    }
    return true;
}
*/
LogicTile World::get_tile(int id)
{
    return *(map->gid_lookup[id]);
}