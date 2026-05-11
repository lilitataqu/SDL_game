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
//写成虚类，可以让子类方法重写
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
void PokemonState::handleEvent(SDL_Event& e,Tex_Manager *tex)
{
    if(e.key.keysym.sym == SDLK_g)
    {
        tex->ui[MENU].able = true;
        tex->ui[MENU_SELECT_BOX].able = true;
        tex->ui[MENU_POKEMON_BG].able = false;
        tex->ui[MENU_POKEMON_BG_BOX].able = false;
        mgr->pop();
    }
}
void PokemonState::render(Game*game, Tex_Manager* tex, World* world, Player* player)
{
    draw_ui(game,tex);
}
//************************************************************************************************** */
//菜单界面
//************************************************************************************************** */
void MenuState::handleEvent(SDL_Event& e,Tex_Manager *tex)
{
    if(e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_g)
    mgr->pop();
    if(e.key.keysym.sym == SDLK_f)
    switch (options[mgr->menuCursor])
    {
    case MenuOption::POKEMON:
        tex->ui[MENU].able = false;
        tex->ui[MENU_SELECT_BOX].able = false;
        tex->ui[MENU_POKEMON_BG].able = true;
        tex->ui[MENU_POKEMON_BG_BOX].able = true;
        mgr->push(std::make_unique<PokemonState>(mgr));
        break;
    
    default:
        break;
    }
}
void MenuState::handleInput(Tex_Manager *tex)
{
    //tex->move_mens_box();
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    uint32_t now = SDL_GetTicks();
    if(tex->ui[MENU_SELECT_BOX].rect.y >= 79 + tex->ui[0].rect.y && (keys[SDL_SCANCODE_UP ]|| keys[SDL_SCANCODE_W]))
    {
        if (tex->menu_box.firstPress)
        {
            tex->ui[1].rect.y -= 58;  // 第一次立即触发
            mgr->menuCursor -= 1;
            tex->menu_box.firstPress = false;
            tex->menu_box.lastTime = now;
        } 
        else
        {
            if (now - tex->menu_box.lastTime > 200) {
                tex->ui[1].rect.y -= 58;
                mgr->menuCursor -= 1;
                tex->menu_box.lastTime = now - (80);
            }
        }
        return;
    }
    else if (tex->ui[MENU_SELECT_BOX].rect.y <= 21+58*5+tex->ui[0].rect.y && (keys[SDL_SCANCODE_DOWN ]|| keys[SDL_SCANCODE_S]))
    {
        if (tex->menu_box.firstPress)
        {
            tex->ui[1].rect.y += 58;  // 第一次立即触发
            mgr->menuCursor += 1;
            tex->menu_box.firstPress = false;
            tex->menu_box.lastTime = now;
        } 
        else
        {
            if (now - tex->menu_box.lastTime > 200) {
                tex->ui[1].rect.y += 58;
                mgr->menuCursor += 1;
                tex->menu_box.lastTime = now - (80);
            }
        }
        return;
    }
    else{
        tex->menu_box.firstPress = true;
    }
    return;
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

/********************************************************* */
//处理键盘输入函数
/********************************************************* */
void input(Game *game,StateManager *state,Tex_Manager *tex,World *world,Player *player)
{
    SDL_Event event;
    while (SDL_PollEvent(&(event))) 
        {
            //判断事件类型 响应菜单事件，如关闭游戏，打开设置
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
