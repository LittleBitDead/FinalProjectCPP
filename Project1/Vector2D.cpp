#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include "Vector.cpp"
#include <math.h>

class Vector2D : public Vector {
private:
    float x;
    float y;

public:
    //Constructors
    Vector2D(float x, float y) {
        this->x = x;
        this->y = y;
    }
    Vector2D() : x(0), y(0) {}

    //Getter and Setter
    float getX() { return x; }
    float getY() { return y; }
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }

    //Vector based methods
    float magnitude() override {
        return sqrt(x*x + y*y);
    }
    virtual void normalize () override {
        float mag = magnitude();
        x = x/mag;
        y = y/mag;
    }
    void scalarMultiply(float scale) override {
        x *= scale;
        y *= scale;
    }
    Vector* scalarMult(float scale) override {
        Vector2D temp(x * scale, y * scale);
        return &temp;
    }

    void add(Vector *v) override {
        Vector2D *temp = (Vector2D*) v;
        x += temp->x;
        y += temp->y;
    }

    void sub(Vector *v) override {
        Vector2D *temp = (Vector2D*) v;
        x -= temp->x;
        y -= temp->y;
    }

    float dotProduct(Vector *v) override {
        Vector2D *temp = (Vector2D* ) v;
        return x * temp->getX() + y * temp->getY();
    }

    //Base Class
    std::string toString() {
        std::stringstream out;
        out << '(' << x << ", " << y << ')';
        return out.str();
    }
    bool compareTo(Vector2D *v) {
        return x == v->x && y == v->y;
    }
};

