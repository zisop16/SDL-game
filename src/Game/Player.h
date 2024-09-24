#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Character.h"
#include "Sprite.h"

class Player {
    public:
    Player(SDL_Texture* spritesheet) {
        SDL_Rect playerSpriteSource;
        playerSpriteSource.x = 0;
        playerSpriteSource.y = 25;
        playerSpriteSource.w = 16;
        playerSpriteSource.h = 6;
        sprite = new Sprite(spritesheet, playerSpriteSource);
        Vec2 initialPosition (8, 2);
        float height = .5;
        float width = height * 16. / 6;
        playerCharacter = new Character(sprite, initialPosition, 1, .5);
        // meters per second
        speed = 3;
    }
    void Draw() {
        playerCharacter->Draw();
    }
    // direction is a unit vector pointing in a cardinal direction
    void Move(Vec2 direction) {
        playerCharacter->Position += direction * speed * Values.DeltaTime;
    }
    ~Player() {
        delete sprite;
    }
    float speed;

    private:
    Character* playerCharacter;
    Sprite* sprite;
    
};

#endif