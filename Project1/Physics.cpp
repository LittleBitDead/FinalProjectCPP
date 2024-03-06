#pragma once
#include "Vector2D.cpp"
#include <simplecpp>

class Physics {
protected:
    bool staticObj = false;

    float mass = 1.0;
    float intertia = 1.0;

    float drag = 0.0;
    float dragRot = 0.0;

    Vector2D velocity;
    Vector2D deltaVelocity;
    Vector2D acceleration;

    float velocityRot = 0;
    float deltaVelocityRot = 0;
    float accelerationRot = 0;

public:
    void setVel(Vector2D* v) {
        velocity = *v;
    }

    float getMass() {
        return mass;
    }
    //Force generated on the origin
    void applyForce(Vector2D &force) {
        force.scalarMultiply(1.0/(mass));
        acceleration.add(&force);
    }
    //Force generated away from the origin on the object
    //resulting in torque
    void applyTorque(float Torque) {

    }

    virtual void calcMotion(float delta) {
        if (staticObj) return;
        velocity.add(&acceleration);
        velocity.scalarMultiply((1.0-drag * delta));

        deltaVelocity = velocity;
        deltaVelocity.scalarMultiply(delta);



        acceleration.scalarMultiply(0);
    }
};
