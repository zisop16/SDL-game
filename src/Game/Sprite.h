#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameValues.h"

class Sprite {
    public:
    Sprite(SDL_Texture* tex, SDL_Rect source) {
        this->source = source;
        this->tex = tex;
    }
    void Draw(float x, float y, float width, float height) {
        SDL_Rect target = Values.MetersToPixels(x, y, width, height);
        SDL_RenderCopy(Values.Renderer, tex, &source, &target);
    }
    private:
    SDL_Rect source;
    SDL_Texture* tex;
};

#endif