#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "Sprite.h"

class UserInterface {
    public:
    UserInterface() {
        SDL_Rect heartSpriteSource;
        heartSpriteSource.x = 51;
        heartSpriteSource.y = 20;
        heartSpriteSource.w = 8;
        heartSpriteSource.h = 7;
        heartSprite = new Sprite(Values.Spritesheet, heartSpriteSource);
    }
    void Draw() {
        DrawHearts();
    }
    private:
    void DrawHearts() {
        float scale = .5;
        float widthRatio = 8./7;
        float spacing = .15;
        float x = .5;
        float y = 8.5;
        heartSprite->Draw(x, y, scale * widthRatio, scale);
        heartSprite->Draw(x + widthRatio * scale * (1 + spacing), y, scale * widthRatio, scale);
        heartSprite->Draw(x + 2 * widthRatio * scale * (1 + spacing), y, scale * widthRatio, scale);
    }
    Sprite* heartSprite;
};

#endif