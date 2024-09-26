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
        SDL_Rect gameOverSource;
        gameOverSource.x = 0;
        gameOverSource.y = 0;
        gameOverSource.w = 1280;
        gameOverSource.h = 720;
        gameOverSprite = new Sprite(Values.Gameover, gameOverSource);
    }
    void Update() {
        if (Values.PlayerDead()) {
            float respawnWidth = 8.2;
            float respawnHeight = .8;
            float buttonX = 8.1;
            float buttonY = 3.95;
            float minX = buttonX - respawnWidth/2;
            float maxX = buttonX + respawnWidth/2;
            float minY = buttonY - respawnHeight/2;
            float maxY = buttonY + respawnHeight/2;
            float x = Values.MousePosition.x;
            float y = Values.MousePosition.y;
            bool hover = x >= minX && x <= maxX && y >= minY && y <= maxY;
            if (hover) {
                gameOverSprite->SetTexture(Values.GameoverRespawn);
                if (Values.LeftClick && !Values.LeftClickRegistered) {
                    Values.LeftClickRegistered = true;
                    Values.ResetGame = true;
                }
            } else {
                gameOverSprite->SetTexture(Values.Gameover);
            }
        }
    }
    void Draw() {
        if (Values.PlayerDead()) {
            gameOverSprite->Draw(8, 4.5, 16, 9);
            return;
        }
        DrawHearts();
    }
    ~UserInterface() {
        delete heartSprite;
        delete gameOverSprite;
    }
    private:
    void DrawHearts() {
        float scale = .5;
        float widthRatio = 8./7;
        float spacing = .15;
        float x = .5;
        float y = 8.5;
        int heartsDrawn = Values.Hearts;
        if (Values.PlayerInvulnerability()) {
            heartsDrawn += Values.FlashState();
        } 
        for (int i = 0; i < heartsDrawn; i++) {
            heartSprite->Draw(x + i * widthRatio * scale * (1 + spacing), y, scale * widthRatio, scale);
        }
    }
    Sprite* heartSprite;
    Sprite* gameOverSprite;
};

#endif