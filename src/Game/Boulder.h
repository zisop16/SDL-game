#ifndef BOULDER_H
#define BOULDER_H

#include "Sprite.h"
#include "Vec2.h"
#include "Character.h"
#include "Collidable.h"

class Boulder : public Collidable {
    public:
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
        height = source.h / 10.;
        width = source.w / 10.;
        rotation = 0;
        this->position = position;
        sprite = new Sprite(Values.Spritesheet, source);
        boulderCharacter = new Character(sprite, position, width, height);
    }
    void Draw() {
        boulderCharacter->Draw();
    }
    void Update() {
        boulderCharacter->Position += Velocity * Values.DeltaTime;
        position = boulderCharacter->Position;
    }
    Vec2 Velocity;
    ~Boulder() {
        delete sprite;
        delete boulderCharacter;
    }
    private:
    Sprite* sprite;
    Character* boulderCharacter;
};

#endif