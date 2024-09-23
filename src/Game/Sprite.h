#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameValues.h"

class Sprite {
    public:
    Sprite(GameValues* values, SDL_Texture* tex, SDL_Rect* source) {
        this->values = values;
        this->source = source;
        this->tex = tex;
    }
    void Draw(float x, float y, float width, float height) {
        SDL_Rect* target = values->MetersToPixels(x, y, width, height);
        SDL_RenderCopy(values->Renderer, tex, source, target);
    }
    private:
    GameValues* values;
    SDL_Rect* source;
    SDL_Texture* tex;
};

#endif