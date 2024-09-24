#ifndef GAMEVALUES_H
#define GAMEVALUES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vec2.h"

class GameValues {
    public:
    SDL_Renderer* Renderer;
    SDL_Texture* BackgroundTexture;
    SDL_Texture* Spritesheet;
    int Width;
    int Height;
    Vec2 MovementLastFrame;
    bool VerticalMovementSwapped;
    bool HorizontalMovementSwapped;
    // Time measured in seconds
    float LastFrameTime;
    float CurrentTime;
    float DeltaTime;

    /**
     * We will force the game to be 16:9, 16 meters wide and 9 meters tall
     */
    SDL_Rect MetersToPixels(float x, float y, float width, float height) {
        const float MetersWide = 16;
        const float MetersHigh = 9;
        float actualRatio = static_cast<float>(Width) / Height;
        float targetRatio = MetersWide / MetersHigh;
        float ratioRatio = actualRatio / targetRatio;
        // If monitor is 20:9, ratioRatio is >1, so all lengths will be multiplied. Otherwise, all lengths will be shrunk.
        int xPixels = static_cast<int>((x - width/2) * ratioRatio / MetersWide * Width);
        // We invert y so that (0, 0) is at the bottom left
        // Also we subtract width/2, height/2 from position to make the positions at center of rect
        int yPixels = static_cast<int>((MetersHigh - y - height/2) * Height / MetersHigh);
        int widthPixels = static_cast<int>(width * ratioRatio / MetersWide * Width);
        int heightPixels = static_cast<int>(height * Height / MetersHigh);
        SDL_Rect pixels;
        pixels.x = xPixels;
        pixels.y = yPixels;
        pixels.w = widthPixels;
        pixels.h = heightPixels;
        return pixels;
    }
};

int Random(int floor, int max) {
    int range = max - floor + 1;
    return rand() % range + floor;
}

GameValues Values;

#endif