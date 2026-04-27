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
    MENU,
    MENU_SELECT_BOX,
    UI_NUM
}UI;

struct KeyRepeat {
    Uint32 lastTime = 0;
    bool firstPress = true;
};

typedef struct
{
    SDL_Texture *tex;
    SDL_Rect rect;
    bool able = false;
    int layer = 0;
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
    std::vector<std::string> tileset_png_paths;
    std::vector<std::string> bg_paths;
    std::vector<std::string> pokemon_paths;
    std::vector<std::string> ui_paths;
    KeyRepeat menu_box;
public:
    TEX btl_bg[BATTLE_BG_NUM];
    TEX pokemon_tex[BATTLE_POKEMON_NUM];
    TEX ui[UI_NUM];
    std::vector<SDL_Texture*> tiles;
    SDL_Texture *player;
    void move_mens_box();
    Tex_Manager(SDL_Renderer* renderer);
    ~Tex_Manager();
};
#endif