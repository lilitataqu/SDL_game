#ifndef TEX_MANAGER
#define TEX_MANAGER

#include <SDL.h>
#include <SDL_image.h>
#include <unordered_map>
#include <string>
#include <vector>
typedef enum
{
    BATTLE_GRASSLAND,
    BATTLE_BG_NUM

}BATTLE_BG_ID;

typedef enum
{
    CHARMANDER,
    SQUIRTLE,
    BATTLE_POKEMON_NUM
}BATTLE_POKEMON;

typedef enum
{
    CLOTH,
    UI_NUM
}UI;

typedef struct
{
    SDL_Texture *tex;
    SDL_Rect rect;
    int able;
    int layer;
}TEX;
/*
typedef struct
{
    TEX btl_bg[1];
    TEX pokemon_tex[2];
    TEX ui[1];
}TEXS;*/
class Tex_Manager
{
private:
    
    std::vector<std::string> bg_paths;
    std::vector<std::string> pokemon_paths;
    std::vector<std::string> ui_paths;
public:
    TEX btl_bg[BATTLE_BG_NUM];
    TEX pokemon_tex[BATTLE_POKEMON_NUM];
    TEX ui[UI_NUM];
    Tex_Manager(SDL_Renderer* renderer);
    ~Tex_Manager();
};
#endif