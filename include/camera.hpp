// Camera.hpp
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL2/SDL_rect.h>

class Camera {
public:
    Camera(int viewW, int viewH);
    
    void follow(const SDL_Point& targetPos, int mapW, int mapH);
    void clamp(int mapW, int mapH);
    
    SDL_Rect getRect() const { return m_rect; }
    int getX() const { return m_rect.x; }
    int getY() const { return m_rect.y; }
    void setViewOffset(int x, int y) { m_viewX = x; m_viewY = y; }

private:
    SDL_Rect m_rect;      // 摄像机视口
    int m_viewX = 0;      // 视野中心偏移 X
    int m_viewY = 0;      // 视野中心偏移 Y
};

#endif
