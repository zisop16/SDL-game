#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "Vec2.h"
#include <cstdlib>
#include <math.h>

/**
 * Determine if two rectangles collide
 */
bool rectangleCollide(Vec2 pos1, Vec2 pos2, float w1, float w2, float h1, float h2, float theta1, float theta2) {
    const float epsilon = .01;
    // Check if the angles are exactly equal, or if they are pi radians apart
    bool equalAngle = ((theta2 - theta1) < epsilon) || ((theta2 - std::fmod(theta1 + M_PI, 2 * M_PI)) < epsilon);
    if (equalAngle) {

    }
    return false;
}

class Collidable {
    public:
    float width;
    float height;
};

#endif