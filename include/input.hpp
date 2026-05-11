#ifndef INPUT_H
#define INPUT_H
#include <stack>
#include <memory>
#include "game.hpp"
#include "world.hpp"
#include "player.hpp"
#include "tex_manager.hpp"
/****************************************************************** */
//一些游戏状态下，接下来可能的游戏状态枚举
/******************************************************************** */
//菜单
enum class MenuOption {
    BAG,         //背包
    POKEMON,    //宝可梦
    TRAINER,    //训练家
    SETTING,    //设置
    RECORD,     //记录
    SAVE,       //保存
    EXIT        //退出
};

/******************************************************************* */
//游戏状态类，这是父类，子类通过调用父类方法
/******************************************************************** */

class GameState {
public:
    virtual void handleEvent(SDL_Event& e,Tex_Manager *tex) = 0;   // 一次性输入（按键触发），纯虚，必须继承才行
    virtual void handleInput(Tex_Manager *tex);               // 持续输入（长按）
    virtual void player_move(Player *player,World *world,Tex_Manager *tex);
    virtual void render(Game*, Tex_Manager*, World*, Player*);  //draw

    virtual ~GameState() = default;
};
/********************************************************************************************* */
//状态管理器
/********************************************************************************************* */
class StateManager {
private:
    

    std::stack<std::unique_ptr<GameState>> states;
public:
    StateManager();
    void push(std::unique_ptr<GameState> s);

    void pop();
    //获取栈顶元素
    GameState* current();

    //一些状态需要记录光标位置
    int menuCursor = 0;
};
/********************************************************************************************* */
//具体的状态类
/********************************************************************************************* */
class PokemonState : public GameState {
private:
    StateManager* mgr;
    int x = 0, y = 0; // 2D格子

public:
    PokemonState(StateManager* m) : mgr(m) {}

    void handleEvent(SDL_Event& e,Tex_Manager *tex) override ;
    void render(Game*, Tex_Manager*, World*, Player*) override;
};

//菜单状态
class MenuState : public GameState {
private:
    StateManager* mgr;
    std::vector<MenuOption> options = {
    MenuOption::BAG,         //背包
    MenuOption::POKEMON,    //宝可梦
    MenuOption::TRAINER,    //训练家
    MenuOption::SETTING,    //设置
    MenuOption::RECORD,     //记录
    MenuOption::SAVE,       //保存
    MenuOption::EXIT        //退出
    };  //选择的状态

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