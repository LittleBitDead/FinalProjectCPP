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
    double r;
    double x, y;
    double vx, vy;
    double ax, ay;
    double mass;
    double charge;
};

//Hooke's Law
// SF = -k * delta(D)
struct Spr {
    double x, y;
    double length;
    double stiffness;
    Cir c;
};

class PhysicsSim : public Window {
private:
    double elasticity = 1;
    bool borderCollision = true;
    vector<Cir> circles;
    vector<Circle> circlesG;
    function<void(Cir &c1, Cir &c2)> forceBTWc;
    function<void(Cir &c)> generalF;

    double distance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    void checkBoarder(Cir &c) {
        if(!borderCollision) return;
        if (c.x - c.r < posX - width/2) {
            c.x = posX - width/2 + c.r;
            c.vx *= -elasticity;
        }
        if (c.x + c.r > posX + width/2) {
            c.x = posX + width/2 - c.r;
            c.vx *= -elasticity;
        }
        if (c.y - c.r < posY - height/2) {
            c.y = posY - height/2 + c.r;
            c.vy *= -elasticity;
        }
        if (c.y + c.r > posY + height/2) {
            c.y = posY + height/2 - c.r;
            c.vy *= -elasticity;
        }
    }

public:
    PhysicsSim(int x, int y, int w, int h, string n) :
    Window(x,y,w,h,n) { }

    int getW() {return width;}
    int getH() {return height;}

    void setElasticity(double e) {
        if (e<0.0 || e>1.0) return;
        elasticity = e;
    }
    void setBorder(bool b) {
        borderCollision = b;
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
            cg.setColor(COLOR(200 - (int)(c.mass - c.charge),200 - c.mass,200 - c.mass));
        if (c.charge < 0)
            cg.setColor(COLOR(200 - (int)c.mass,200 - (int)c.mass,200 - (int)(c.mass + c.charge)));
        if (c.charge == 0)
            cg.setColor(COLOR(200 - (int)c.mass,200 - (int)c.mass,200 - (int)c.mass));

        circlesG.push_back(cg);

    }

    void updatePhysics(double delta) {
        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i].ax = 0;
            circles[i].ay = 0;
        }
        if(forceBTWc != NULL) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    if (forceBTWc != NULL)
                        forceBTWc(circles[i], circles[j]);
                }
                if (generalF != NULL)
                    generalF(circles[i]);
            }
        }

        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i].vx += circles[i].ax * delta;
            circles[i].vy += circles[i].ay * delta;
            circles[i].x += circles[i].vx * delta;
            circles[i].y += circles[i].vy * delta;
        }
    }

    void collisionDetection() {
        for (unsigned int i = 0; i < circles.size(); i++) {
            Cir &c1 = circles[i];
            checkBoarder(c1);
            for (unsigned int j = i + 1; j < circles.size(); j++) {
                //Collision Happens
                float d = distance(circles[i].x, circles[i].y, circles[j].x, circles[j].y);
                if (d < circles[i].r + circles[j].r) {
                    //Resolve overlap
                    double dx = circles[j].x - circles[i].x;
                    double dy = circles[j].y - circles[i].y;
                    double angle = atan2(dy, dx);
                    double xOff = cos(angle) * (circles[i].r + circles[j].r - d);
                    double yOff = sin(angle) * (circles[i].r + circles[j].r - d);
                    circles[i].x -= xOff/2;
                    circles[i].y -= yOff/2;
                    circles[j].x += xOff/2;
                    circles[j].y += yOff/2;
                    //Resolve Momentum
                    //Normals
                    d = distance(circles[i].x, circles[i].y, circles[j].x, circles[j].y);
                    double nx = dx/d;
                    double ny = dy/d;
                    double relV = (circles[j].vx - circles[i].vx) * nx + (circles[j].vy - circles[i].vy) * ny;
                    double imp = 2 * relV / (1.0/circles[i].mass + 1.0/circles[j].mass);
                    circles[i].vx += imp/circles[i].mass * nx;
                    circles[i].vy += imp/circles[i].mass * ny;
                    circles[j].vx -= imp/circles[j].mass * nx;
                    circles[j].vy -= imp/circles[j].mass * ny;
                }
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
        borderCollision = true;
        endFrame();
    }

    void handleIteract(int x, int y) {
        if (x < posX - width/2) return;
        if (x > posX + width/2) return;
        if (y < posY - height/2) return;
        if (y > posY + height/2) return;

        for (unsigned int i = 0; i < circles.size(); i++) {
            Cir &c = circles[i];
            if (distance(x,y,c.x,c.y) < c.r) {
                XEvent event;
                event.xmotion.x = 0;
                event.xmotion.y = 0;
                int posRx = posX - width/2;
                int posRy = posY - height/2;
                stringstream mass;
                mass << "Mass: " << c.mass;
                Text tMass(posRx + textWidth(mass.str())/ 2, posRy + textHeight(), mass.str());
                stringstream charge;
                charge << "Charge: " << c.charge;
                Text tCharge(posRx + textWidth(charge.str())/ 2, posRy + textHeight() * 2, charge.str());
                stringstream posXY;
                posXY << "Position (X,Y): (" << c.x << ", " << c.y << ")";
                Text tPosXY(posRx + textWidth(posXY.str())/ 2, posRy + textHeight() * 3, posXY.str());
                stringstream velXY;
                velXY << "Velocity (X,Y): (" << c.vx << ", " << c.vy << ")";
                Text tVelXY(posRx + textWidth(velXY.str())/ 2, posRy + textHeight() * 4, velXY.str());
                stringstream accXY;
                accXY << "Acceleration (X,Y): (" << c.ax << ", " << c.ay << ")";
                Text tAccXY(posRx + textWidth(accXY.str())/ 2, posRy + textHeight() * 5, accXY.str());

                Line l(c.x,c.y,c.x,c.y);
                Line velLine(c.x,c.y, c.x + c.vx, c.y + c.vy);
                Line accLine(c.x,c.y, c.x + c.ax, c.y + c.ay);
                while (true) {
                    if (checkEvent(event)) {
                        if (mouseButtonReleaseEvent(event)) {
                            //In the event the user clicks off
                            if (event.xmotion.x == 0 && event.xmotion.y == 0) return;
                            c.vx = event.xmotion.x - c.x;
                            c.vy = event.xmotion.y - c.y;
                            return;
                        }
                    }
                    //Update Data
                    posXY.str("");
                    posXY << "Position (X,Y): (" << c.x << ", " << c.y << ")";
                    tPosXY.reset(posRx + textWidth(posXY.str())/ 2, posRy + textHeight() * 3, posXY.str());
                    velXY.str("");
                    velXY << "Velocity (X,Y): (" << c.vx << ", " << c.vy << ")";
                    tVelXY.reset(posRx + textWidth(velXY.str())/ 2, posRy + textHeight() * 4, velXY.str());
                    accXY.str("");
                    accXY << "Acceleration (X,Y): (" << c.ax << ", " << c.ay << ")";
                    tAccXY.reset(posRx + textWidth(accXY.str())/ 2, posRy + textHeight() * 5, accXY.str());
                    //Update Lines
                    velLine.reset(c.x,c.y, c.x + c.vx, c.y + c.vy);
                    accLine.reset(c.x,c.y, c.x + c.ax, c.y + c.ay);
                    if (event.xmotion.x == 0 && event.xmotion.y == 0) {
                        l.reset(c.x, c.y, c.x, c.y);
                    } else {
                        l.reset(c.x, c.y, event.xmotion.x, event.xmotion.y);
                    }

                    //SlowMotion
                    updatePhysics(0.01);
                    collisionDetection();
                    updateGraphics();
                }
            }
        }
    }
};
