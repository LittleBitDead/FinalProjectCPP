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
    void handleClick(int x, int y, PhysicsSim* p) {
        if (x < posX - width/2) return;
        if (x > posX + width/2) return;
        if (y < posY - height/2) return;
        if (y > posY + height/2) return;
        for (unsigned int i = 0; i < btns.size(); i++) {
            if (btns[i]->containsPoint(x, y)) {
                btns[i]->click(p);
                break;
            }
        }
    }
};

struct Cir {
    float r;
    float x, y;
    float vx, vy;
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
    function<void(Cir &c1, Cir &c2)> force;

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
    void setForceFunction(function<void(Cir &c1, Cir &c2)> func) {
        force = func;
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
        if(force != NULL) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    force(circles[i], circles[j]);
                }
            }
        }

        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i].x += circles[i].vx * delta;
            circles[i].y += circles[i].vy * delta;
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
        force = NULL;
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
            Line l;
            if (distance(x,y,c.x,c.y) < c.r) {
                while (true) {
                    XEvent event;
                    nextEvent(event);
                    if (mouseDragEvent(event)) {
                        l.reset(c.x,c.y, event.xmotion.x, event.xmotion.y);
                    }
                    if(mouseButtonReleaseEvent(event)){
                        c.vx = (event.xmotion.x - c.x) / 2;
                        c.vy = (event.xmotion.y - c.y) / 2;
                        l.hide();
                        return;
                    }
                }
            }
        }
    }
};
