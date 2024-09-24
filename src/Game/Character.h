#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Sprite.h"
#include "Vec2.h"
#include "Collidable.h"

class Character: public Collidable {
    public:
    Character() {

    }
    Character(Sprite* sprite, Vec2 position, float width, float height) {
        this->position = position;
        this->width = width;
        this->height = height;
        this->rotation = 0;
        this->sprite = sprite;
    }
    void Draw() {
        sprite->Draw(position.x, position.y, width, height, rotation);
    }
    Sprite* sprite;
    private:
    float speed;
};

#endif