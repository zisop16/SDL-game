#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Character.h"
#include "Sprite.h"
#include "Collidable.h"

class Player : public Character {
    public:
    Player() {
        SDL_Rect playerSpriteSource;
        playerSpriteSource.x = 0;
        playerSpriteSource.y = 25;
        playerSpriteSource.w = 16;
        playerSpriteSource.h = 6;
        sprite = new Sprite(Values.Spritesheet, playerSpriteSource);
        position.x = 8;
        position.y = 2;
        float scale = .5;
        height = scale;
        width = scale * 16. / 6;
        rotation = 0;
        // meters per second
        speed = 3;
    }
    void Draw() {
        Character::Draw();
    }
    void Update() {
        
    }
    // direction is a unit vector pointing in a cardinal direction
    void Move(Vec2 direction) {
        position += direction * speed * Values.DeltaTime;

    }
    ~Player() {
        delete sprite;
    }
    float speed;
    
};

#endif