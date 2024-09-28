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
        position.y = 4.5;
        float scale = .5;
        height = scale;
        width = scale * 16. / 6;
        rotation = 0;
        // meters per second^2
        acceleration = 25;
        lastShotTime = 0;
        // 1 shot per cooldown(seconds)
        shotCooldown = .25;
    }
    void Draw() {
        for (int i = 0; i < Bullets.size(); i++) {
            Bullets.at(i)->Draw();
        }
        bool dontDraw = Values.PlayerDead() || Values.PlayerInvulnerability() && !Values.FlashState();
        if (!dontDraw) {
            Character::Draw();
        }
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

        Bullets.push_back(fired);
    }
    bool ShotOffCooldown() {
        return (Values.CurrentTime - lastShotTime) >= shotCooldown;
    }
    void ChargeShot() {
        lastShotTime = 0;
    }
    void DeleteProjectile(int ind) {
        Projectile* erased = Bullets.at(ind);
        Bullets.erase(std::next(Bullets.begin(), ind));
        delete erased;
    }
    void Update() {
        if (!Values.PlayerDead()) {
            Move();
            if (Values.LeftClick && !Values.LeftClickRegistered && ShotOffCooldown()) {
                Values.LeftClickRegistered = true;
                Shoot();
            }
        }
        std::vector<int> toErase;
        for (int i = Bullets.size() - 1; i >= 0; i--) {
            Bullets.at(i)->Update();
            if (Bullets.at(i)->OffScreen()) {
                toErase.push_back(i);
            }
        }
        for (int ind : toErase) {
            DeleteProjectile(ind);
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
        float exponent = 1.25;
        float dragConstant = acceleration * std::pow(maxSpeed, -exponent);
        Vec2 dragForce = -velocity * dragConstant * std::pow(velocity.Length(), .5);
        velocity += dragForce * Values.DeltaTime;
    }
    ~Player() {
        delete sprite;
        for (int i = 0; i < Bullets.size(); i++) {
            delete Bullets.at(i);
        }
    }
    std::vector<Projectile*> Bullets;
    private:
    float acceleration;
    Vec2 velocity;
    float lastShotTime;
    float shotCooldown;
};

#endif