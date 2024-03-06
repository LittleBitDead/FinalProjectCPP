#pragma once
#include "PolyShape.cpp"
#include "Vector2D.cpp"
#include "Physics.cpp"
#include "GameObject.cpp"

class PolyGameObject : public GameObject {
private:
    PolyShape p;
    Line velDir;
public:
    PolyGameObject(float _mass, float _drag, Vector2D *_points, int _n, Vector2D &_origin) : p(_points, _n, _origin) {
        mass = _mass;
        drag = _drag;
    }

    PolyGameObject(float _mass, float _drag, Vector2D *_points, int _n) : p(_points, _n) {
        mass = _mass;
        drag = _drag;
        velDir.reset(p.getOrigin()->getX(), p.getOrigin()->getY(),
               p.getOrigin()->getX(), p.getOrigin()->getY());
    }

    void updatePhysics(float delta) override {
        Physics::calcMotion(delta);
        p.moveShape(deltaVelocity);
        p.rotateShape(deltaVelocityRot);
    }

    void updateGraphics() override {
        p.draw();
        float x = p.getOrigin()->getX();
        float y = p.getOrigin()->getY();
        float toX = p.getOrigin()->getX() + velocity.getX() / 2;
        float toY = p.getOrigin()->getY() + velocity.getY() / 2;
        velDir.reset(x,y,toX,toY);
    }

    Shape* getShape() {
        return &p;
    }

    std::string toString() override {
        std::stringstream out;
        out << "Poly" << endl;
        out << "Mass: " << mass << endl;
        out << "Drag: " << drag << endl;
        out << p.toString();
        return out.str();
    }
};
