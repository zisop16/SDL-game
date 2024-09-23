#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.h"
#include "GameValues.h"

const std::string SPRITES_LOCATION = "../../../sprites/";

class Game {
    public:
    Game() {

    }
    
    bool Initialize() {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        // values->Width = DM.w;
        // values->Height = DM.h;
        values->Width = 1600;
        values->Height = 900;
        window = SDL_CreateWindow("Classic Game", 100, 100, values->Width, values->Height, SDL_WINDOW_ALLOW_HIGHDPI);
        // SDL_SetWindowFullscreen(window, SDL_TRUE);
        if (window == NULL) {
            std::cout << "Could not create window: " << SDL_GetError() << std::endl;
            return false;
        }
        
        values->Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (values->Renderer == NULL) {
            std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
            return false;
        }
        
        SDL_Texture* spritesheet = IMG_LoadTexture(values->Renderer, (SPRITES_LOCATION + "pico8_invaders_sprites.png").c_str());
        player = new Player(values, spritesheet);
        
        return true;

    }
    void RunLoop() {
        SDL_Event event;
        while (true) {
            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    break;
                }
                if (event.type == SDL_KEYDOWN) {
                    std::cout << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                }
            }
        }
        SDL_SetRenderDrawColor(values->Renderer, 255, 255, 255, 255);
        SDL_RenderClear(values->Renderer);

        player->Draw();

        SDL_RenderPresent(values->Renderer);
    }
    void Shutdown() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    GameValues* values;

    ~Game() {
        delete player;
    }
    
    private:
    SDL_Window* window;
    Player* player;
};

#endif