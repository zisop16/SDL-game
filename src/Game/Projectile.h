#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Character.h"

class Projectile : public Character {
    public:
    Projectile(int type) {
        SDL_Rect source;
        switch(type) {
            case 0: {
                source.x = 8;
                source.y = 12;
                source.w = 4;
                source.h = 4;
                break;
            }
            case 1: {
                source.x = 12;
                source.y = 12;
                source.w = 4;
                source.h = 4;
                break;
            }
        }
        sprite = new Sprite(Values.Spritesheet, source);
        float scale = .2;
        height = scale;
        width = scale;
        position = Zero;
        rotation = 0;
    }
    ~Projectile() {
        delete sprite;
    }
};

#endif