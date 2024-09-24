#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "Vec2.h"
#include <cstdlib>
#include <math.h>
/**
 * Determine the point at which two lines intersect
 * m1x + b1 = m2x + b2
 * x(m2 - m1) = b1 - b2
 * x = (b1 - b2) / (m2 - m1)
 */
Vec2 lineIntersection(float m1, float b1, float m2, float b2) {
    float x = (b1 - b2) / (m2 - m1);
    float y = m1 * x;
    Vec2 intersection(x, y);
    return intersection;
}

/**
 * Determine if a line segment, given by l1 -> l2, intersects a rotated rectangle
 * Does not work for parallel lines
 */
bool lineSegmentCollidesRectangle(Vec2& pos, float w, float h, float theta, Vec2& l1, Vec2& l2) {
    // Ensure that l1 is the left vector
    if (l1.x > l2.x) {
        Vec2& temp = l1;
        l1 = l2;
        l2 = temp;
    }
    float m = (l2.y - l1.y) / (l2.x - l1.x);
    float b = l1.y - m * l1.x;
    Vec2 horizDirection(theta);
    Vec2 vertDirection(theta + M_PI / 2);
    float mHoriz = horizDirection.y / horizDirection.x;
    float mVert = -1/mHoriz;
    
    // Corners of the rectangle
    Vec2 pos11 = pos - horizDirection * w/2 + vertDirection * h/2;
    Vec2 pos12 = pos + horizDirection * w/2 + vertDirection * h/2;
    Vec2 pos21 = pos - horizDirection * w/2 - vertDirection * h/2;
    Vec2 pos22 = pos + horizDirection * w/2 - vertDirection * h/2;

    // y intercept of all perimeter lines
    float bHoriz1 = pos11.y - mHoriz * pos11.x;
    float bHoriz2 = pos21.y - mHoriz * pos21.x;
    float bVert1 = pos11.y - mVert * pos11.x;
    float bVert2 = pos12.y - mVert * pos12.x;
    Vec2 horizInt1 = lineIntersection(m, b, mHoriz, bHoriz1);
    Vec2 horizInt2 = lineIntersection(m, b, mHoriz, bHoriz2);
    Vec2 vertInt1 = lineIntersection(m, b, mVert, bVert1);
    Vec2 vertInt2 = lineIntersection(m, b, mVert, bVert2);
    
    float leftX = pos11.x;
    float rightX = pos12.x;
    if (leftX > rightX) {
        leftX = pos12.x;
        rightX = pos11.x;
    }
    if (horizInt1.x >= leftX && horizInt1.x <= rightX && horizInt1.x >= l1.x && horizInt1.x <= l2.x) {
        return true;
    }
    
    leftX = pos21.x;
    rightX = pos22.x;
    if (leftX > rightX) {
        leftX = pos22.x;
        rightX = pos21.x;
    }
    if (horizInt2.x >= leftX && horizInt2.x <= rightX && horizInt2.x >= l1.x && horizInt2.x <= l2.x) {
        return true;
    }
    leftX = pos11.x;
    rightX = pos21.x;
    if (leftX > rightX) {
        leftX = pos21.x;
        rightX = pos11.x;
    }
    if (vertInt1.x >= leftX && vertInt2.x <= rightX && vertInt1.x >= l1.x && vertInt1.x <= l2.x) {
        return true;
    }
    leftX = pos12.x;
    rightX = pos22.x;
    if (leftX > rightX) {
        leftX = pos22.x;
        rightX = pos12.x;
    }
    if (vertInt2.x >= leftX && vertInt2.x <= rightX && vertInt2.x >= l1.x && vertInt2.x <= l2.x) {
        return true;
    }

    return false;
}

/**
 * Determine if two rectangles collide
 */
bool rectangleCollide(Vec2& pos1, Vec2& pos2, float w1, float w2, float h1, float h2, float theta1, float theta2) {
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
    // Check if rectangles are pi/2 radians apart
    float rotatedTheta1 = std::fmod(theta1 + M_PI / 2, 2 * M_PI);
    bool equalIfRotated = (std::abs(theta2 - rotatedTheta1) < epsilon) || (std::abs(theta2 - std::fmod(theta1 + M_PI, 2 * M_PI)) < epsilon);
    if (equalIfRotated) {
        Vec2 diff = (pos2 - pos1);
        float horizontalDistance = std::abs(diff.dot(horiz1));
        bool horizontalCondition = horizontalDistance <= (w1/2 + h2/2);
        float verticalDistance = std::abs(diff.dot(vert1));
        bool verticalCondition = verticalDistance <= (h1/2 + w2/2);
        return horizontalCondition && verticalCondition;
    }

    float horiz1Slope = horiz1.y / horiz1.x;
    float vert1Slope = -1/horiz1Slope;
    Vec2 pos11 = pos1 + horiz1 * w1/2 + vert1 * h1/2;
    Vec2 pos12 = pos1 + horiz1 * w1/2 - vert1 * h1/2;
    Vec2 pos21 = pos1 - horiz1 * w1/2 + vert1 * h1/2;
    Vec2 pos22 = pos1 - horiz1 * w1/2 - vert1 * h1/2;

    // Check if any 4 line segments on rect1 collide with rect2
    bool coll1 = lineSegmentCollidesRectangle(pos2, w2, h2, theta2, pos11, pos12);
    bool coll2 = lineSegmentCollidesRectangle(pos2, w2, h2, theta2, pos12, pos22);
    bool coll3 = lineSegmentCollidesRectangle(pos2, w2, h2, theta2, pos22, pos21);
    bool coll4 = lineSegmentCollidesRectangle(pos2, w2, h2, theta2, pos21, pos11);

    return coll1 || coll2 || coll3 || coll4;
}

/**
 * Basic collidable for a rectangular hitbox
 * Only checks if perimeters are intersecting
 */
class Collidable {
    public:
    float width;
    float height;
    float rotation;
    Vec2 position;

    bool Colliding(Collidable& other) {
        return rectangleCollide(position, other.position, width, other.width, height, other.height, rotation, other.rotation);
    }
};

#endif