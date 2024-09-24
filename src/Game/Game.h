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
#include "Projectile.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

const std::string SPRITES_LOCATION = "./sprites/";

class Game {
    public:
    Game() {

    }
    
    bool Initialize() {
        srand((unsigned)time(NULL));
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
        Vec2 boulderPosition(5, 4);
        boulder = new Boulder();
        boulder->position = boulderPosition;
        proj = new Projectile(1);
        proj->position.x = 5;
        proj->position.y = 5;
        
        return true;

    }
    void Update() {
        const Uint8* keyPresses = SDL_GetKeyboardState(NULL);
        int mouseX, mouseY;
        Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
        Values.LeftClick = mouseButtons & 0b1;
        // We have a LeftClickRegistered bool stored
        // This will prevent 1 click from being registered multiple times
        // When the button is held down
        if (!Values.LeftClick) {
            Values.LeftClickRegistered = false;
        }
        Vec2 position(mouseX, mouseY);
        Values.MousePosition = position;
        
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
        proj->Draw();

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
            if (Values.DeltaTime > .25) {
                Values.DeltaTime = .25;
            }
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
        delete proj;
    }
    
    private:
    SDL_Window* window;
    Player* player;
    Background* background;
    Boulder* boulder;
    Projectile* proj;
};

#endif