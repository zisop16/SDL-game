#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Character.h"
#include "Sprite.h"
#include "Collidable.h"

class Player : public Collidable {
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
        rotation = 0;
        height = .5;
        width = height * 16. / 6;
        playerCharacter = new Character(sprite, position, 1, .5);
        // meters per second
        speed = 3;
    }
    void Draw() {
        playerCharacter->Draw();
    }
    void Update() {
        
    }
    // direction is a unit vector pointing in a cardinal direction
    void Move(Vec2 direction) {
        playerCharacter->Position += direction * speed * Values.DeltaTime;
        position = playerCharacter->Position;
    }
    ~Player() {
        delete sprite;
        delete playerCharacter;
    }
    float speed;

    private:
    Character* playerCharacter;
    Sprite* sprite;
    
};

#endif