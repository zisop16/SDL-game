#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2 {
    public:
    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
    float x;
    float y;
    float Length() {
        return sqrt(x * x + y * y);
    }
};

#endif