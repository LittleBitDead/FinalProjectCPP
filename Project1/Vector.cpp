#pragma once
#include "Object.cpp"

class Vector : public Object {

public:
    virtual float magnitude() { return 0.0f; }
    virtual void normalize() {}
    virtual void scalarMultiply (float scale) {}
    virtual Vector* scalarMult (float scale) {}
    virtual void add(Vector *v) {}
    virtual void sub(Vector *v) {}
    virtual float dotProduct(Vector *v) { return 0.0f; }
};
