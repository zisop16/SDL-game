#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.h"
#include "Background.h"
#include "GameValues.h"
#include "Boulder.h"
#include <filesystem>
#include "Vec2.h"

const std::string SPRITES_LOCATION = "./sprites/";

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
        Values.Width = 1600;
        Values.Height = 900;
        window = SDL_CreateWindow("Classic Game", 100, 100, Values.Width, Values.Height, SDL_WINDOW_ALLOW_HIGHDPI);
        // SDL_SetWindowFullscreen(window, SDL_TRUE);
        if (window == NULL) {
            std::cout << "Could not create window: " << SDL_GetError() << std::endl;
            return false;
        }
        
        Values.Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (Values.Renderer == NULL) {
            std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
            return false;
        }
        std::string sprites = SPRITES_LOCATION + "pico8_invaders_sprites.png";
        std::string stars = SPRITES_LOCATION + "background.png";
        Values.Spritesheet = IMG_LoadTexture(Values.Renderer, sprites.c_str());
        Values.BackgroundTexture = IMG_LoadTexture(Values.Renderer, stars.c_str());
        player = new Player();
        background = new Background();
        Vec2 boulderVelocity(0, 0);
        Vec2 boulderPosition(5, 4);
        boulder = new Boulder(boulderPosition, boulderVelocity, 4);
        
        return true;

    }
    void Update() {
        const Uint8* keyPresses = SDL_GetKeyboardState(NULL);
        bool moveUp = keyPresses[SDL_SCANCODE_W];
        bool moveDown = keyPresses[SDL_SCANCODE_S];
        bool moveLeft = keyPresses[SDL_SCANCODE_A];
        bool moveRight = keyPresses[SDL_SCANCODE_D];
        Vec2 movementVector;
        bool movingUp = Values.MovementLastFrame.y  > 0;
        bool movingDown = Values.MovementLastFrame.y  < 0;
        bool movingRight = Values.MovementLastFrame.x > 0;
        bool movingLeft = Values.MovementLastFrame.x < 0;
        if (moveUp && moveDown) {
            if (!Values.VerticalMovementSwapped) {
                // If we were moving down last frame, we will swap directions
                if (Values.MovementLastFrame.y < 0) {
                    movementVector.y = 1;
                } else {
                    movementVector.y = -1;
                }
                Values.VerticalMovementSwapped = true;
            } else {
                if (Values.MovementLastFrame.y < 0) {
                    movementVector.y = -1;
                } else {
                    movementVector.y = +1;
                }
            }
        } else if (moveUp) {
            movementVector.y = 1;
            Values.VerticalMovementSwapped = false;
        } else if (moveDown) {
            movementVector.y = -1;
            Values.VerticalMovementSwapped = false;
        }
        if (moveLeft && moveRight) {
            if (!Values.HorizontalMovementSwapped) {
                if (Values.MovementLastFrame.x < 0) {
                    movementVector.x = 1;
                } else {
                    movementVector.x = -1;
                }
                Values.HorizontalMovementSwapped = true;
            // If we have already swapped movement direction, we will persist the movement direction we swapped to
            } else {
                if (Values.MovementLastFrame.x < 0) {
                    movementVector.x = -1;
                } else {
                    movementVector.x = 1;
                }
            }
        } else if (moveLeft) {
            movementVector.x = -1;
            Values.HorizontalMovementSwapped = false;
        } else if (moveRight) {
            movementVector.x = 1;
            Values.HorizontalMovementSwapped = false;
        }
        if (movementVector != Zero) {
            movementVector.Normalize();
        }
        player->Move(movementVector);
        Values.MovementLastFrame = movementVector;
        background->Update();

        
    }
    void Draw() {
        SDL_RenderClear(Values.Renderer);

        background->Draw();
        player->Draw();
        boulder->Draw();

        SDL_RenderPresent(Values.Renderer);
        
    }
    void RunLoop() {
        SDL_Event event;
        Values.LastFrameTime = SDL_GetTicks() / 1000.;
        while (true) {
            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    break;
                }
            }

            

            Values.CurrentTime = SDL_GetTicks() / 1000.;
            Values.DeltaTime = Values.CurrentTime - Values.LastFrameTime;
            Update();
            Draw();
            Values.LastFrameTime = Values.CurrentTime;
        }
    }
    void Shutdown() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    ~Game() {
        delete player;
        delete background;
        delete boulder;
    }
    
    private:
    SDL_Window* window;
    Player* player;
    Background* background;
    Boulder* boulder;
};

#endif