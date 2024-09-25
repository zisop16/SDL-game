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
#include "UserInterface.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

const std::string SPRITES_LOCATION = "./sprites/";

class Game {
    public:
    
    bool Initialize(std::mt19937 RNG) {
        Values.gen = RNG;
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
        UI = new UserInterface();

        // One boulder per MeanBoulderTime sec
        Values.MeanBoulderTime = .5;
        Values.NextBoulder = Values.RandExp(1/Values.MeanBoulderTime);

        Values.Hearts = 3;
        
        return true;

    }
    void HandleInputs() {
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
        // MouseY is inversed so that 0 == bottom of screen, 9 == top of screen
        // We scale both coordinates so that x == 0 -> 16, y == 0 -> 9
        Vec2 position(mouseX * 16. / Values.Width, 9 - (mouseY * 9. / Values.Height));
        Values.MousePosition = position;
        
        bool moveUp = keyPresses[SDL_SCANCODE_W];
        bool moveDown = keyPresses[SDL_SCANCODE_S];
        bool moveLeft = keyPresses[SDL_SCANCODE_A];
        bool moveRight = keyPresses[SDL_SCANCODE_D];
        bool movingUp = Values.MovementLastFrame.y  > 0;
        bool movingDown = Values.MovementLastFrame.y  < 0;
        bool movingRight = Values.MovementLastFrame.x > 0;
        bool movingLeft = Values.MovementLastFrame.x < 0;
        Values.PlayerMovementVector = Zero;
        if (moveUp && moveDown) {
            if (!Values.VerticalMovementSwapped) {
                // If we were moving down last frame, we will swap directions
                if (Values.MovementLastFrame.y < 0) {
                    Values.PlayerMovementVector.y = 1;
                } else {
                    Values.PlayerMovementVector.y = -1;
                }
                Values.VerticalMovementSwapped = true;
            } else {
                if (Values.MovementLastFrame.y < 0) {
                    Values.PlayerMovementVector.y = -1;
                } else {
                    Values.PlayerMovementVector.y = +1;
                }
            }
        } else if (moveUp) {
            Values.PlayerMovementVector.y = 1;
            Values.VerticalMovementSwapped = false;
        } else if (moveDown) {
            Values.PlayerMovementVector.y = -1;
            Values.VerticalMovementSwapped = false;
        }
        if (moveLeft && moveRight) {
            if (!Values.HorizontalMovementSwapped) {
                if (Values.MovementLastFrame.x < 0) {
                    Values.PlayerMovementVector.x = 1;
                } else {
                    Values.PlayerMovementVector.x = -1;
                }
                Values.HorizontalMovementSwapped = true;
            // If we have already swapped movement direction, we will persist the movement direction we swapped to
            } else {
                if (Values.MovementLastFrame.x < 0) {
                    Values.PlayerMovementVector.x = -1;
                } else {
                    Values.PlayerMovementVector.x = 1;
                }
            }
        } else if (moveLeft) {
            Values.PlayerMovementVector.x = -1;
            Values.HorizontalMovementSwapped = false;
        } else if (moveRight) {
            Values.PlayerMovementVector.x = 1;
            Values.HorizontalMovementSwapped = false;
        }
        if (Values.PlayerMovementVector != Zero) {
            Values.PlayerMovementVector.Normalize();
        }
    }
    void HandleBoulders() {
        Values.NextBoulder -= Values.DeltaTime;
        if (Values.NextBoulder <= 0) {
            Values.NextBoulder = Values.RandExp(1/Values.MeanBoulderTime);
            FireBoulder();
        }
        std::vector<int> toErase;
        for (int i = 0; i < boulders.size(); i++) {
            Boulder* b = boulders.at(i);
            b->Update();
            if (b->OffScreen()) {
                toErase.push_back(i);
            }
        }
        for (int ind : toErase) {
            boulders.erase(std::next(boulders.begin(), ind));
        }
    }
    void FireBoulder() {
        float lowerBound = 4;
        float upperBound = 6;
        float speed = Values.RandUnif(lowerBound, upperBound);
        int side = Values.RandInt(0, 2);
        float direction = 0;
        // We don't want boulders flying parallel to the screen, so we will have a buffer at the edge of each angle
        // We will make angles uniformly distributed, and spawn positions normally distributed to be more likely to spawn
        // At the center of the screen side
        float buffer = .3;
        Boulder* toFire = new Boulder();
        toFire->rotation = Values.RandUnif(0, 2 * M_PI);
        toFire->AngularVelocity = Values.RandNormal(0, 2);
        float x;
        float y;
        switch(side) {
            case 0: {
                // left
                direction = Values.RandUnif(-M_PI / 2 + buffer, M_PI / 2 - buffer);
                y = Values.RandNormal(4.5, .5);
                x = -toFire->width/2;
                break;
            }
            case 1: {
                // up
                direction = Values.RandUnif(M_PI + buffer, 2 * M_PI - buffer);
                x = Values.RandNormal(8, .3);
                y = toFire->height/2 + 9;
                break;
            }
            case 2: {
                // right
                direction = Values.RandUnif(M_PI / 2 + buffer, 3 * M_PI / 2 - buffer);
                y = Values.RandNormal(4.5, .5);
                x = 16 + toFire->width/2;
                break;
            }
        }
        toFire->position.x = x;
        toFire->position.y = y;
        Vec2 directionVector(direction);
        toFire->Velocity = directionVector * speed;
        boulders.push_back(toFire);
    }
    void Update() {
        HandleInputs();
        HandleBoulders();

        background->Update();
        player->Update();

        Values.MovementLastFrame = Values.PlayerMovementVector;
    }
    void Draw() {
        SDL_RenderClear(Values.Renderer);

        background->Draw();
        player->Draw();
        for (Boulder* b : boulders) {
            b->Draw();
        }
        UI->Draw();

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
        delete UI;
    }
    
    private:
    SDL_Window* window;
    UserInterface* UI;
    Player* player;
    Background* background;
    std::vector<Boulder*> boulders;
};

#endif