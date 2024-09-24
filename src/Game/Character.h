#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Sprite.h"
#include "Vec2.h"

class Character {
    public:
    Character(Sprite* sprite, Vec2 position, float width, float height) {
        this->Position = position;
        this->width = width;
        this->height = height;
        this->sprite = sprite;
    }
    void Draw() {
        SDL_Rect* target;
        sprite->Draw(Position.x, Position.y, width, height);
    }
    Vec2 Position;
    private:
    Sprite* sprite;
    float speed;
    float width;
    float height; 
};

#endif