#pragma once
#include "GameObject.cpp"
#include "PolyGameObject.cpp"
#include "CircleGameObject.cpp"
#include <vector>

class Simulation {
protected:
    vector<GameObject*> gameObjects;
    int left, right, top, bottom;
    const float delta = 0.02;
    const float e = 1;

public:

    Simulation(int width, int height) {
        left = 0;
        right = width;
        top = 0;
        bottom = height;
    }

    void mainLoop() {
        collisionDetection();
        applyForceAll();
        drawAll();
    }
    void applyForceAll() {
        Vector2D v(0,0);
        for (unsigned int i = 0; i < gameObjects.size(); i++) {
            gameObjects[i]->applyForce(v);
            gameObjects[i]->updatePhysics(delta);
        }
        wait(delta);
    }

    void drawAll() {
        for (unsigned int i = 0; i < gameObjects.size(); i++) {
            gameObjects[i]->updateGraphics();
        }
    }

    void collisionDetection() {
        for (unsigned int i = 0; i < gameObjects.size(); i++) {
            borderResolve(gameObjects[i]);
        }
        for (unsigned int i = 0; i < gameObjects.size(); i++) {
            for (unsigned int j = i + 1; j < gameObjects.size(); j++) {
                detectCollision(gameObjects[i], gameObjects[j]);
            }
        }
    }

    void borderResolve(GameObject *obj) {
        if (typeid(*obj) == typeid(PolyGameObject)) {
                borderResolve((PolyGameObject*) obj);
            } else {
                borderResolve((CircleGameObject*) obj);
            }
    }

    void borderResolve(PolyGameObject *obj) {

    }

    void borderResolve(CircleGameObject *obj) {
        CircleShape *c = (CircleShape*)obj->getShape();
        Vector2D* o = c->getOrigin();
        if (o->getX() + c->getMaxR() > right) {
            o->setX(right - c->getMaxR() - 1);
            obj->getVel()->setX(obj->getVel()->getX() * -1);
        } else if (o->getX() - c->getMaxR() < left) {
            o->setX(left + c->getMaxR() + 1);
            obj->getVel()->setX(obj->getVel()->getX() * -1);
        }

        if (o->getY() + c->getMaxR() > bottom) {
            o->setY(bottom - c->getMaxR() - 1);
            obj->getVel()->setY(obj->getVel()->getY() * -1);
        } else if (o->getY() - c->getMaxR() < top) {
            o->setY(top + c->getMaxR() + 1);
            obj->getVel()->setY(obj->getVel()->getY() * -1);
        }
    }

    void detectCollision(GameObject *obj1, GameObject *obj2) {
        resolveCollision((CircleGameObject*) obj1, (CircleGameObject*) obj2);
    }

    void resolveCollision(CircleGameObject *c1, CircleGameObject *c2) {
        CircleShape* s1 = (CircleShape*)c1->getShape();
        CircleShape* s2 = (CircleShape*)c2->getShape();

        Vector2D v(s2->getOrigin()->getX() - s1->getOrigin()->getX(),
                   s2->getOrigin()->getY() - s1->getOrigin()->getY());

        float totalR = s1->getMaxR() + s2->getMaxR();

        float mag = v.magnitude();

        if (mag < totalR) {
            float dif = totalR - mag;
            v.normalize();
            v.scalarMultiply(dif/2.0f);
            s2->moveShape(v);
            v.scalarMultiply(-1);
            s1->moveShape(v);
            float totalMass = c1->getMass() + c2->getMass();
        }
    }



    void addGameObject(GameObject* obj) {
        gameObjects.push_back(obj);
    }

};
