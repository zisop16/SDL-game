#ifndef GAMEVALUES_H
#define GAMEVALUES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameValues {
    public:
    SDL_Renderer* Renderer;
    int Width;
    int Height;

    /**
     * We will force the game to be 16:9, 16 meters wide and 9 meters tall
     */
    SDL_Rect* MetersToPixels(float x, float y, float width, float height) {
        float actualRatio = Width / Height;
        float targetRatio = 16. / 9;
        float ratioRatio = actualRatio / targetRatio;
        // If monitor is 20:9, ratioRatio is >1, so all lengths will be multiplied. Otherwise, all lengths will be shrunk.
        int xPixels = static_cast<int>((x - width/2) * ratioRatio / 16 * Width);
        // We invert y so that (0, 0) is at the bottom left
        // Also we subtract width/2, height/2 from position to make the positions at center of rect
        int yPixels = static_cast<int>((1 - y - height/2) * Height / 9);
        int widthPixels = static_cast<int>(width * ratioRatio / 16 * Width);
        int heightPixels = static_cast<int>(height * Height / 9);
        SDL_Rect* pixels;
        pixels->x = xPixels;
        pixels->y = yPixels;
        pixels->w = widthPixels;
        pixels->h = heightPixels;
        return pixels;
    }
};

#endif