#ifndef GAMEVALUES_H
#define GAMEVALUES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vec2.h"
#include <random>


class GameValues {
    public:
    
    SDL_Renderer* Renderer;
    SDL_Texture* BackgroundTexture;
    SDL_Texture* Spritesheet;
    // For boulders that have lost 50% health
    SDL_Texture* TintedSpritesheet;
    SDL_Texture* Gameover;
    // Displayed when hovering gameover
    SDL_Texture* GameoverRespawn;
    int Width;
    int Height;
    int Hearts;
    bool LeftClick;
    bool LeftClickRegistered;
    // Indicates whether game should be reset on next frame
    bool ResetGame;
    Vec2 MousePosition;
    Vec2 PlayerMovementVector;
    Vec2 MovementLastFrame;
    bool VerticalMovementSwapped;
    bool HorizontalMovementSwapped;
    // Time measured in seconds
    float LastFrameTime;
    float CurrentTime;
    // Time when player was hit by an asteroid
    float PlayerHitTime;
    float DeltaTime;
    // Time when next boulder will apear
    float NextBoulder;
    // Mean time before boulders appear
    float MeanBoulderTime;
    // Amount of time between flashes when player has been hit by a boulder
    float FlashTime;
    // Amount of time player spends flashing invulnerable after being hit
    float InvulnerabilityTime;
    int FrameCounter;
    float LastFPSCountTime;
    /*
    Returns 1 if player should be flashed visible, or 0 if player should be flashed invisible during invulnerability frames
    */
    bool FlashState() {
        float diff = CurrentTime - PlayerHitTime;
        // If diff is .5 and flashtime is .2, we take floor(.5 / .2) == 2 % 2 == 0 -> player should be flash off
        int state = static_cast<int>(diff / FlashTime) % 2;
        return state;
    }
    bool PlayerInvulnerability() {
        return CurrentTime - PlayerHitTime <= InvulnerabilityTime;
    }
    bool PlayerDead() {
        return Hearts == 0;
    }

    std::mt19937 gen;

    /**
     * We will force the game to be 16:9, 16 meters wide and 9 meters tall
     */
    SDL_Rect MetersToPixels(float x, float y, float width, float height) {
        const float MetersWide = 16;
        const float MetersHigh = 9;
        // If monitor is 20:9, all lengths will be multiplied. Otherwise, all lengths will be shrunk.
        int xPixels = static_cast<int>((x - width/2) / MetersWide * Width);
        // We invert y so that (0, 0) is at the bottom left
        // Also we subtract width/2, height/2 from position to make the positions at center of rect
        int yPixels = static_cast<int>((MetersHigh - y - height/2) * Height / MetersHigh);
        int widthPixels = static_cast<int>(width / MetersWide * Width);
        int heightPixels = static_cast<int>(height * Height / MetersHigh);
        SDL_Rect pixels;
        pixels.x = xPixels;
        pixels.y = yPixels;
        pixels.w = widthPixels;
        pixels.h = heightPixels;
        return pixels;
    }
    
    int RandInt(int floor, int max) {
        std::uniform_int_distribution distrib(floor, max);
        return distrib(gen);
    }
    float RandExp(float lambda) {
        std::exponential_distribution distrib(lambda);
        return distrib(gen);
    }
    float RandUnif(float floor, float max) {
        std::uniform_real_distribution distrib(floor, max);
        return distrib(gen);
    }
    float RandNormal(float mean, float stdev) {
        std::normal_distribution distrib(mean, stdev);
        return distrib(gen);
    }
};



GameValues Values;

#endif