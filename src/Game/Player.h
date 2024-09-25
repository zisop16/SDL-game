#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Character.h"
#include "Sprite.h"
#include "Collidable.h"
#include "Projectile.h"
#include <vector>
#include <cmath>

class Player : public Character {
    public:
    Player() {
        SDL_Rect playerSpriteSource;
        playerSpriteSource.x = 0;
        playerSpriteSource.y = 25;
        playerSpriteSource.w = 16;
        playerSpriteSource.h = 6;
        sprite = new Sprite(Values.Spritesheet, playerSpriteSource);
        position.x = 8;
        position.y = 2;
        float scale = .5;
        height = scale;
        width = scale * 16. / 6;
        rotation = 0;
        // meters per second^2
        acceleration = 16;
        lastShotTime = 0;
        // 1 shot per cooldown(seconds)
        shotCooldown = .5;
    }
    
    void Draw() {
        for (int i = 0; i < bullets.size(); i++) {
            bullets.at(i)->Draw();
        }
        Character::Draw();
    }
    void Shoot() {
        lastShotTime = Values.CurrentTime;
        Projectile* fired = new Projectile(0);
        Vec2 projectilePosition = position;
        projectilePosition.y += height/2;
        fired->position = projectilePosition;
        Vec2 mouseDisplacement = Values.MousePosition - projectilePosition;
        float angle;
        if (mouseDisplacement.x != 0) {
            angle = std::atan(mouseDisplacement.y / mouseDisplacement.x);
            if (mouseDisplacement.x < 0) {
                angle += M_PI;
            }
        } else {
            if (mouseDisplacement.y > 0) {
                angle = M_PI / 2;
            } else {
                angle = 3 * M_PI / 2;
            }
        }
        Vec2 direction(angle);
        fired->velocity = direction * fired->speed;

        bullets.push_back(fired);
    }
    bool ShotOffCooldown() {
        return (Values.CurrentTime - lastShotTime) >= shotCooldown;
    }
    void Update() {
        Move();
        if (Values.LeftClick && !Values.LeftClickRegistered && ShotOffCooldown()) {
            Values.LeftClickRegistered = true;
            Shoot();
        }
        std::vector<int> toErase;
        for (int i = bullets.size() - 1; i >= 0; i--) {
            bullets.at(i)->Update();
            if (bullets.at(i)->OffScreen()) {
                toErase.push_back(i);
            }
        }
        for (int ind : toErase) {
            bullets.erase(std::next(bullets.begin(), ind));
        }
    }
    void Move() {
        velocity += Values.PlayerMovementVector * acceleration * Values.DeltaTime;
        ApplyDrag();
        position += velocity * Values.DeltaTime;
        if (TouchingBottom()) {
            position.y = height/2;
            velocity.y = 0;
        } else if (TouchingTop()) {
            position.y = 9 - height/2;
            velocity.y = 0;
        }
        if (TouchingLeft()) {
            position.x = width/2;
            velocity.x = 0;
        } else if (TouchingRight()) {
            position.x = 16 - width/2;
            velocity.x = 0;
        }
    }
    /**
     * Drag force will be proportional to velocity^1.5
     * We are targeting a maximum speed
     * v' = a - kv^1.5
     * If v' = 0,
     * a = kv_f^1.5
     * k = a * v_f^(-1.5)
     */
    void ApplyDrag() {
        float maxSpeed = 8;
        float exponent = 1.5;
        float dragConstant = acceleration * std::pow(maxSpeed, -exponent);
        Vec2 dragForce = -velocity * dragConstant * std::pow(velocity.Length(), .5);
        velocity += dragForce * Values.DeltaTime;
    }
    ~Player() {
        delete sprite;
        for (int i = 0; i < bullets.size(); i++) {
            delete bullets.at(i);
        }
        delete heartSprite;
    }
    private:
    float acceleration;
    Vec2 velocity;
    float lastShotTime;
    float shotCooldown;
    std::vector<Projectile*> bullets;
    Sprite* heartSprite;
};

#endif