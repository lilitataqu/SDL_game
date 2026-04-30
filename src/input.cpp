//#include "world.hpp"
//#include "game.hpp"
//#include "tex_manager.hpp"
#include "input.hpp"
#include "render.hpp"
//#include "player.hpp"
#include "config.hpp"



//********************************************************** */
//构建状态虚类
//输入默认空实现，具体由子类实现
//纯虚函数不用写实现
//******************************************************* */
void GameState::handleInput(Tex_Manager *tex){}
void GameState::player_move(Player *player,World *world,Tex_Manager *tex){}
void GameState::render(Game*, Tex_Manager*, World*, Player*) {}
//*************************************************************** */
//这个类存放栈
//栈的方法 
//***************************************************************** */
StateManager::StateManager()
{
    states.push(std::make_unique<WorldState>(this));
}
//入栈
void StateManager::push(std::unique_ptr<GameState> s) 
{
    states.push(std::move(s));
}
//出栈
void StateManager::pop() {
    if (!states.empty()) states.pop();
}
//获取栈顶元素
GameState* StateManager::current() {
    return states.empty() ? nullptr : states.top().get();
}
//************************************************************************************************** */
//宝可梦界面
//************************************************************************************************** */
void PokemonState::handleInput(Tex_Manager *tex)
{
    
}
//************************************************************************************************** */
//菜单界面
//************************************************************************************************** */
void MenuState::handleEvent(SDL_Event& e,Tex_Manager *tex)
{
    if(e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_x)
    mgr->pop();
}
void MenuState::handleInput(Tex_Manager *tex)
{
     tex->move_mens_box();
}
void MenuState::render(Game* game, Tex_Manager* tex, World* world, Player* player)
{
    draw_map(game,world,player,tex);
    draw_ui(game,tex);
}
//**************************************************************************************** */
//世界界面
//************************************************************************************************* */
void WorldState::handleEvent(SDL_Event& e,Tex_Manager *tex)
{
    if(e.key.keysym.sym == SDLK_e)
    mgr->push(std::make_unique<MenuState>(mgr));
}
void WorldState::player_move(Player *player,World *world,Tex_Manager *tex)
{
    player->player_update(world,tex);
}
void WorldState::render(Game* game, Tex_Manager* tex, World* world, Player* player)
{
    draw_map(game,world,player,tex);
}
void input(Game *game,StateManager *state,Tex_Manager *tex,World *world,Player *player)
{
    SDL_Event event;
    while (SDL_PollEvent(&(event))) 
        {
            //相应菜单事件，如关闭游戏，打开设置
           switch (event.type)
            {
            //键盘输入时
            case SDL_KEYDOWN:
                if(state->current())
                state->current()->handleEvent(event,tex);
                //esc
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        game->running = 0;
                    }
                break;   
            //点x关闭游戏
            case SDL_QUIT:
                game->running = 0;
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    float scale = fmin(event.window.data1 / (game->window_w * 1.0f),event.window.data2 / (game->window_h * 1.0f));
                    game->rect.w = scale * game->window_w ,game->rect.h = scale * game->window_h ;
                    game->rect.x = (event.window.data1 - game->rect.w) / 2;
                    game->rect.y = (event.window.data2 - game->rect.h) / 2;
                }
            default:
                break;
            }
            
        }
    if(state->current())
    state->current()->handleInput(tex);
    if(state->current())
    state->current()->player_move(player,world,tex);
    if(state->current())
    state->current()->render(game,tex,world,player);
}
