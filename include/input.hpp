#ifndef INPUT_H
#define INPUT_H
#include <stack>
#include <memory>
#include "game.hpp"
#include "world.hpp"
#include "player.hpp"
#include "tex_manager.hpp"

class GameState {
public:
    virtual void handleEvent(SDL_Event& e,Tex_Manager *tex) = 0;   // 一次性输入（按键触发），纯虚，必须继承才行
    virtual void handleInput(Tex_Manager *tex);               // 持续输入（长按）
    virtual void player_move(Player *player,World *world,Tex_Manager *tex);
    virtual void render(Game*, Tex_Manager*, World*, Player*);  //draw

    virtual ~GameState() = default;
};

class StateManager {
private:
    std::stack<std::unique_ptr<GameState>> states;
public:
    StateManager();
    void push(std::unique_ptr<GameState> s);

    void pop();
    //获取栈顶元素
    GameState* current();
};

class PokemonState : public GameState {
private:
    StateManager* mgr;
    int x = 0, y = 0; // 2D格子

public:
    PokemonState(StateManager* m) : mgr(m) {}

    void handleInput(Tex_Manager *tex) override ;
};

//菜单状态
class MenuState : public GameState {
private:
    StateManager* mgr;
    int cursor = 0;

public:
    MenuState(StateManager* m) : mgr(m) {}

    void handleEvent(SDL_Event& e,Tex_Manager *tex) override;

    void handleInput(Tex_Manager *tex) override;

    void render(Game*, Tex_Manager*, World*, Player*) override;

};

class WorldState : public GameState {
private:
    StateManager* mgr;

public:
    WorldState(StateManager* m) : mgr(m) {}

    void handleEvent(SDL_Event& e,Tex_Manager *tex) override;

    void player_move(Player *player,World *world,Tex_Manager *tex) override;

    void render(Game*, Tex_Manager*, World*, Player*) override;

};

//void ui(Game *game);
void input(Game *game,StateManager *state,Tex_Manager *tex,World *world,Player *player);
#endif