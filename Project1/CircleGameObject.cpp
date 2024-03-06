#pragma once
#include "PolyShape.cpp"
#include "Vector2D.cpp"
#include "Physics.cpp"
#include "CircleShape.cpp"
#include "GameObject.cpp"

class CircleGameObject : public GameObject {
private:
    CircleShape c;
    Line velDir;
public:
    CircleGameObject(float _mass, float _drag, Vector2D &_origin, float _radius) : c(_origin, _radius) {
        mass = _mass;
        drag = _drag;
    }

    void updatePhysics(float delta) override {
        Physics::calcMotion(delta);
        c.moveShape(deltaVelocity);
    }

    void updateGraphics() override {
        c.draw();
        float x = c.getOrigin()->getX();
        float y = c.getOrigin()->getY();
        float toX = c.getOrigin()->getX() + velocity.getX() / velocity.magnitude() * 10;
        float toY = c.getOrigin()->getY() + velocity.getY() / velocity.magnitude() * 10;
        velDir.reset(x,y,toX,toY);
    }

    Shape* getShape() {
        return &c;
    }

    std::string toString() override {
        std::stringstream out;
        out << "Circle" << endl;
        out << "Mass: " << mass << endl;
        out << "Drag: " << drag << endl;
        out << c.toString();
        return out.str();
    }
};
