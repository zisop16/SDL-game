#ifndef BOULDER_H
#define BOULDER_H

#include "Sprite.h"
#include "Vec2.h"
#include "Character.h"
#include "Collidable.h"

class Boulder : public Character {
    public:
    Boulder() : Boulder(Zero, Zero, Values.RandInt(0, 4)) {
    }
    // There are 5 boulders in the spritesheet, so type ranges from 0->5
    Boulder(Vec2 position, Vec2 velocity, int type) {
        SDL_Rect source;
        switch(type) {
            case 0: {
                source.x = 16;
                source.y = 19;
                source.w = 7;
                source.h = 7;
                break;
            }
            case 1: {
                source.x = 23;
                source.y = 16;
                source.w = 12;
                source.h = 12;
                break;
            }
            case 2: {
                source.x = 18;
                source.y = 27;
                source.w = 6;
                source.h = 6;
                break;
            }
            case 3: {
                source.x = 36;
                source.y = 16;
                source.w = 11;
                source.h = 8;
                break;
            }
            case 4: {
                source.x = 35;
                source.y = 24;
                source.w = 16;
                source.h = 8;
                break;
            }
        }
        float scale = 1/10.;
        height = source.h * scale;
        width = source.w * scale;
        rotation = 0;
        AngularVelocity = 0;
        this->position = position;
        sprite = new Sprite(Values.Spritesheet, source);
    }
    void Draw() {
        Character::Draw();
    }
    void Update() {
        position += Velocity * Values.DeltaTime;
        rotation += AngularVelocity * Values.DeltaTime;
    }
    Vec2 Velocity;
    float AngularVelocity;
    ~Boulder() {
        delete sprite;
    }
};

#endif