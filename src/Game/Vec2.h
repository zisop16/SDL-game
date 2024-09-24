#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

class Vec2 {
    public:
    Vec2() {
        x = 0;
        y = 0;
    }
    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
    // Create a unit vector from an angle in radians
    Vec2(float angle) {
        x = std::cos(angle);
        y = std::sin(angle);
    }
    Vec2(const Vec2& v)
    {
        x = v.x;
        y = v.y;
    }
    Vec2& operator += (const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2& operator -= (const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vec2& operator = (const Vec2& other)
    {
        if(this != &other)
        {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    bool operator == (const Vec2& other) {
        if (this == &other) {
            return true;
        }
        if (x == other.x && y == other.y) {
            return true;
        }
        return false;
    }
    bool operator != (const Vec2& other) {
        return !operator==(other);
    }
    Vec2 operator + (const Vec2& other) {
        Vec2 added(x + other.x, y + other.y);
        return added;
    }
    Vec2 operator * (const float scalar) {
        Vec2 mult(x * scalar, y * scalar);
        return mult;
    }
    Vec2 operator / (const float scalar) {
        Vec2 mult(x / scalar, y / scalar);
        return mult;
    }
    Vec2 operator - (const Vec2& other) {
        Vec2 subtracted(x - other.x, y - other.y);
        return subtracted;
    }
    float x;
    float y;
    float Length() {
        return sqrt(x * x + y * y);
    }
    float dot(const Vec2& other) {
        return x * other.x + y * other.y;
    }
    void Normalize() {
        float len = Length();
        x /= len;
        y /= len;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);
};

const Vec2 Zero(0, 0);

std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
    os << '(' << vec.x << ", " << vec.y << ')';
    return os;
}

#endif