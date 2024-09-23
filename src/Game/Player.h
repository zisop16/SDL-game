#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Character.h"
#include "Sprite.h"

class Player {
    public:
    Player(GameValues* values, SDL_Texture* spritesheet) {
        std::cout << "a" << std::endl;
        SDL_Rect playerSpriteSource;
        playerSpriteSource.x = 0;
        playerSpriteSource.y = 20;
        playerSpriteSource.w = 10;
        playerSpriteSource.h = 10;
        sprite = new Sprite(values, spritesheet, &playerSpriteSource);
        Vec2 initialPosition (8, 2);
        playerCharacter = new Character(sprite, &initialPosition, .5, .5);
    }
    void Draw() {
        playerCharacter->Draw();
    }
    ~Player() {
        delete sprite;
    }

    private:
    Character* playerCharacter;
    Sprite* sprite;
    
};

#endif