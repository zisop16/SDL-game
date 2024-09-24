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
    bool equalAngle = (std::abs(theta2 - theta1) < epsilon) || (std::abs(theta2 - std::fmod(theta1 + M_PI, 2 * M_PI)) < epsilon);
    Vec2 horiz1(theta1);
    Vec2 vert1(theta1 + M_PI / 2);
    if (equalAngle) {
        Vec2 diff = (pos2 - pos1);
        float horizontalDistance = std::abs(diff.dot(horiz1));
        bool horizontalCondition = (horizontalDistance <= (w1/2 + w2/2));
        float verticalDistance = std::abs(diff.dot(vert1));
        bool verticalCondition = (verticalDistance <= (h1/2 + h2/2));
        return horizontalCondition && verticalCondition;
    }
    float horiz1Slope = horiz1.y / horiz1.x;
    Vec2 horiz11Origin = pos1 + (vert1 * h1/2);
    float y11Int = horiz11Origin.y - horiz1Slope * horiz11Origin.x;


    return false;
}

class Collidable {
    public:
    float width;
    float height;
};

#endif