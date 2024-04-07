#pragma once
#include <cmath>

struct V2 {
public:
    //For precision using double
    double x, y;
    V2(double x, double y) : x(x), y(y) {}

    V2 operator+(const V2 &other) const {
        return V2(x + other.x, y + other.y);
    }
    void operator+=(const V2 &other) {
        x += other.x;
        y += other.y;
    }
    V2 operator-(const V2 &other) const {
        return V2(x - other.x, y - other.y);
    }
    void operator-=(const V2 &other) {
        x -= other.x;
        y -= other.y;
    }
    V2 operator*(const double scalar) const {
        return V2(x * scalar, y * scalar);
    }
    void operator*=(const double scalar) {
        x *= scalar;
        y *= scalar;
    }

    //Distance Op
    double operator<(const V2 &other) const {
        return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
    }

    double mag() const {
        return sqrt(x*x + y*y);
    }

    double dot(const V2 &other) const {
        return x * other.x + y * other.y;
    }

};
