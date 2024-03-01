#pragma once
#include "Object.cpp"

class Vector : public Object {
protected:
    virtual float dotProductV(const Vector &v1, const Vector &v2) { return 0.0f; }
    virtual Vector* crossProductV(const Vector &v1, const Vector &v2) { return nullptr; }
    virtual Vector* addV(const Vector &v1, const Vector &v2) { return nullptr; }
    virtual float angleV(const Vector &v1, const Vector &v2) { return 0.0f; }
    virtual Vector* projectV(const Vector &v1, const Vector &v2) { return nullptr; }

public:
    virtual float magnitude() { return 0.0f; }
    virtual void normalize() {}
    virtual void scalarMultiply(float scale) {}

    float dotProduct(const Vector &v1, const Vector &v2) {
        Vector temp;
        return temp.dotProductV(v1, v2);
    }

    Vector crossProduct(const Vector &v1, const Vector &v2) {
        Vector temp;
        return *temp.crossProductV(v1, v2);
    }

    Vector add(const Vector &v1, const Vector &v2) {
        Vector temp;
        return *temp.addV(v1, v2);
    }

    float angle(const Vector &v1, const Vector &v2) {
        Vector temp;
        return temp.angleV(v1, v2);
    }

    Vector project(const Vector &v1, const Vector &v2) {
        Vector temp;
        return *temp.projectV(v1, v2);
    }
};
