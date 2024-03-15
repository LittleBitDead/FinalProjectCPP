#pragma once
#include <simplecpp>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

#include "Button.cpp"

class Window {
protected:
    int posX, posY;
    int width, height;
    vector<Button*> btns;
    Text label;
    Rectangle box;
public:
    Window(int x, int y, int w, int h, string n) :
    posX(x), posY(y), width(w), height(h) {
        box = Rectangle(posX, posY, width, height);
        label = Text(posX, posY - height/5 * 2, n);
        label.setZIndex(100);
    }
    void addButton(Button* btn) {
        btns.push_back(btn);
    }
    void handleClick(int x, int y) {
        if (x < posX - width/2) return;
        if (x > posX + width/2) return;
        if (y < posY - height/2) return;
        if (y > posY + height/2) return;
        for (unsigned int i = 0; i < btns.size(); i++) {
            if (btns[i]->containsPoint(x, y)) {
                btns[i]->click();
                break;
            }
        }
    }
};

struct Cir {
    float r;
    float x, y;
    float vx, vy;
    float ax, ay;
    float mass;
    float charge;
};

//Hooke's Law
// SF = -k * delta(D)
struct Spr {
    float x, y;
    float length;
    float stiffness;
    Cir c;
};

class PhysicsSim : public Window {
private:
    float elasticity = 1;
    vector<Cir> circles;
    vector<Circle> circlesG;
    function<void(Cir &c1, Cir &c2)> forceBTWc;
    function<void(Cir &c)> generalF;

    float distance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    void checkBoarder(Cir &c) {
        if (c.x - c.r < posX - width/2) {
            c.x = posX - width/2 + c.r;
            c.vx *= -1;
        }
        if (c.x + c.r > posX + width/2) {
            c.x = posX + width/2 - c.r;
            c.vx *= -1;
        }
        if (c.y - c.r < posY - height/2) {
            c.y = posY - height/2 + c.r;
            c.vy *= -1;
        }
        if (c.y + c.r > posY + height/2) {
            c.y = posY + height/2 - c.r;
            c.vy *= -1;
        }
    }

public:
    PhysicsSim(int x, int y, int w, int h, string n) :
    Window(x,y,w,h,n) { }

    int getW() {return width;}
    int getH() {return height;}

    void setElasticity(float e) {
        elasticity = e;
    }
    void setForceFunctionBTWc(function<void(Cir &c1, Cir &c2)> func) {
        forceBTWc = func;
    }
    void setForceFunctionGeneral(function<void(Cir &c)> func) {
        generalF = func;
    }

    void addObjects(Cir c) {
        c.x += posX;
        c.y += posY;
        circles.push_back(c);
        Circle cg(c.x, c.y, c.r);
        cg.setFill(true);
        if (c.charge > 0)
            cg.setColor(COLOR(255, 0,0));
        if (c.charge < 0)
            cg.setColor(COLOR(0,0,255));
        if (c.charge == 0)
            cg.setColor(COLOR(0,0,0));
        circlesG.push_back(cg);

    }

    void updatePhysics(float delta) {
        if(forceBTWc != NULL) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    forceBTWc(circles[i], circles[j]);
                }
                if (generalF != NULL) generalF(circles[i]);
            }
        }

        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i].vx += circles[i].ax / circles[i].mass;
            circles[i].vy += circles[i].ay / circles[i].mass;
            circles[i].x += circles[i].vx * delta;
            circles[i].y += circles[i].vy * delta;
            circles[i].ax = 0;
            circles[i].ay = 0;
        }
    }

    void collisionDetection() {
        for (unsigned int i = 0; i < circles.size(); i++) {
            Cir &c1 = circles[i];
            checkBoarder(c1);
            for (unsigned int j = i + 1; j < circles.size(); j++) {
                Cir &c2 = circles[j];
            }
        }
    }

    void updateGraphics() {
        beginFrame();
        for (unsigned int i = 0; i < circles.size(); i++) {
            circlesG[i].reset(circles[i].x, circles[i].y, circles[i].r);
        }
        endFrame();
    }

    void clearSim() {
        beginFrame();
        circles.clear();
        circlesG.clear();
        forceBTWc = NULL;
        generalF = NULL;
        elasticity = 1;
        endFrame();
    }

    void handleIteract(int x, int y) {
        if (x < posX - width/2) return;
        if (x > posX + width/2) return;
        if (y < posY - height/2) return;
        if (y > posY + height/2) return;
        for (unsigned int i = 0; i < circles.size(); i++) {
            Cir &c = circles[i];
            Line l(c.x,c.y,c.x,c.y);
            if (distance(x,y,c.x,c.y) < c.r) {
                XEvent event;
                event.xmotion.x = 0;
                event.xmotion.y = 0;
                nextEvent(event);
                while (true) {
                    if (checkEvent(event)) {
                        if (mouseDragEvent(event)) {
                            l.reset(c.x, c.y, event.xmotion.x, event.xmotion.y);
                        }
                        if (mouseButtonReleaseEvent(event)) {
                            c.vx = event.xmotion.x - c.x;
                            c.vy = event.xmotion.y - c.y;
                            return;
                        }
                    }
                    //This makes sure that
                    //The XEvents are properly updated
                    //DO NOT CHANGE
                    wait(0.002);
                }
            }
        }
    }
};
