#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Sprite.h"
#include "Vec2.h"

class Background {
    public:
    Background(SDL_Texture* tex) {
        width = 3000;
        height = 1500;
        SDL_Rect source;
        source.x = 0; source.y = 0;
        source.w = width; source.h = height;
        sprite = new Sprite(tex, source);
        velocity.x = 1;
        velocity.y = .4;
    }
    void Draw() {
        // Assuming background sprite is 3000x1500, then it is 9 meters tall and 18 meters wide
        
        float shiftedX;
        if (position.x > 8) {
            shiftedX = position.x - widthMeters();
        } else {
            shiftedX = position.x + widthMeters();
        }
        float shiftedY;
        if (position.y > 4.5) {
            shiftedY = position.y - 9;
        } else {
            shiftedY = position.y + 9;
        }
        sprite->Draw(position.x, position.y, width / height * 9, 9);
        sprite->Draw(shiftedX, position.y, width / height * 9, 9);
        sprite->Draw(position.x, shiftedY, width / height * 9, 9);
        sprite->Draw(shiftedX, shiftedY, width / height * 9, 9);
        // sprite->
    }
    void Update() {
        // If the player moves right, the background moves left??
        Vec2 realVelocity = velocity;
        position += realVelocity * Values.DeltaTime;
        // Reset x position if the image is being drawn too far left or too far right
        float meterWidth = widthMeters();
        float maxX = 16 + meterWidth / 2;
        float minX = -meterWidth / 2;
        if (position.x > maxX) {
            position.x -= meterWidth;
        } else if (position.x < minX) {
            position.x += meterWidth;
        }
        float maxY = 9 + 9./2;
        float minY = -9./2;
        if (position.y > maxY) {
            position.y -= 9;
        } else if (position.y < minY) {
            position.y += 9;
        }
    }
    ~Background() {
        delete sprite;
    }
    private:
    float widthMeters() {
        return static_cast<float>(width) / height * 9;
    }
    Vec2 position;
    Vec2 velocity;
    int width;
    int height;
    Sprite* sprite;
};

#endif