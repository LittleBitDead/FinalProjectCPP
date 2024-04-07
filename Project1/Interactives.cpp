#include <graphics.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <functional>
#include <vector>
#include <conio.h>
#include <thread>

#include "V2.cpp"
#include "PhysicsObjects.cpp"

#define ESCAPE_CHAR 27
#define MOUSE_LMB_DOWN 513

class Element {
protected:
    int posX, posY;
    int width, height;
public:
    int getPosX() { return posX; }
    int getPosY() { return posY; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    void setPosX(int newX) { posX = newX; }
    void setPosY(int newY) { posX = newY; }
    void setWidth(int newW) { width = newW;  }
    void setHeight(int newH) { height = newH; ; }
    virtual void onClick(int x, int y) = 0;
    virtual void draw() = 0;
    bool clickCheck(int x, int y) {
        if (x < posX - width/2 || x > posX + width/2) return false;
        if (y < posY - height/2 || y > posY + height/2) return false;
        return true;
    }
    void setXY(int x, int y) {
        posX = x;
        posY = y;
    }
    int getX() { return posX; }
    int getY() { return posY; }
};

class Button : public Element {
    function<void()> func = NULL;
    char *text;

public:
    Button(int x, int y, int w, int h, char* str, function<void()> func) : func(func) {
        posX = x;
        posY = y;
        text = str;
        width = w;
        height = h;
    }

     void onClick(int x, int y) override {
        if (clickCheck(x,y)) {
            cout << "BUTTON BOX CLICK" << endl;
            if (func != NULL) func();
        }
    }

    virtual void draw() override {
        setcolor(colors::BLACK);
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
        setfillstyle(SOLID_FILL, DARKGRAY);
        setbkcolor(DARKGRAY);
        rectangle(posX - width/2, posY - height,
                  posX + width/2, posY + height/2);
        floodfill(posX, posY, BLACK);

        setcolor(WHITE);
        settextjustify(CENTER_TEXT, VCENTER_TEXT);
        outtextxy(posX, posY, text);
    }
};

class InputBox: public Element {
protected:
    static bool lock;
private:
    char *text;
    double data;

    void userInput() {
        lock = true;
        char key;
        while (true) {
            if (kbhit()) {
                key = getch();
                if (key == ESCAPE_CHAR) return;
                if (ismouseclick(MOUSE_LMB_DOWN)) return;
                else {
                    cout << key << endl;
                }
            }
        }
        lock = false;
    }

public:
    InputBox(int x, int y, int w, int h, char* str) {
        posX = x;
        posY = y;
        width = w;
        height = h;
        text = str;
    }



    void onClick(int x, int y) override {
        if (clickCheck(x,y)) {
            cout << "INPUT BOX CLICK" << endl;
            cout << lock << endl;
            if (!lock) {
                std::thread getInput(&InputBox::userInput, this);
                getInput.detach();
            }
        }
    }

    virtual void draw() override {
        setcolor(colors::BLACK);
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
        setfillstyle(SOLID_FILL, DARKGRAY);
        setbkcolor(DARKGRAY);
        rectangle(posX - width/2, posY - height,
                  posX + width/2, posY + height/2);
        floodfill(posX, posY, BLACK);

        setcolor(WHITE);
        settextjustify(CENTER_TEXT, VCENTER_TEXT);
        outtextxy(posX, posY, text);
    }
};

bool InputBox::lock = false;

class Window : public Element {
private:
    vector<Element*> btns;
    Button btn;

public:
    Window(int x, int y, int w, int h, char* c) : btn(x, y - h/2 + textheight(c) * 2, textwidth(c) + 10, textheight(c), c, NULL) {
        posX = x;
        posY = y;
        width = w;
        height = h;
    }

    void addButton(Element *el) {
        el->setXY(posX-width/2 + el->getX(), posY - height/2 + el->getY());
        btns.push_back(el);
    }

    virtual void onClick(int x, int y) override {
        for (unsigned int i = 0; i < btns.size(); i++) {
            btns[i]->onClick(x, y);
        }
    }

    virtual void draw() override {
        //Title
        btn.draw();
        setcolor(colors::BLACK);
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
        //Top
        line(posX - width/2, posY - height/2, posX + width/2, posY - height/2);
        //Right
        line(posX + width/2, posY - height/2, posX + width/2, posY + height/2);
        //Bottom
        line(posX - width/2, posY + height/2, posX + width/2, posY + height/2);
        //Left
        line(posX - width/2, posY - height/2, posX - width/2, posY + height/2);

        for (unsigned int i = 0; i < btns.size(); i++) {
            btns[i]->draw();
        }
    }
};

class ObjectSim : public Window {
private:
    double elasticity = 1;
    bool borderCollision = true;
    vector<Circle*> circles;
    vector<Joint2D*> joints;
    function<void(Circle &c1, Circle &c2)> forceBTWc;
    function<void(Circle &c)> generalF;

    void checkBoarder(Circle &c) {
        if(!borderCollision) return;
        if (c.pos.x - c.r < posX - width/2) {
            c.pos.x = posX - width/2 + c.r;
            c.vel.x *= -elasticity;
        }
        if (c.pos.x + c.r > posX + width/2) {
            c.pos.x = posX + width/2 - c.r;
            c.vel.x *= -elasticity;
        }
        if (c.pos.y - c.r < posY - height/2) {
            c.pos.y = posY - height/2 + c.r;
            c.vel.y *= -elasticity;
        }
        if (c.pos.y + c.r > posY + height/2) {
            c.pos.y = posY + height/2 - c.r;
            c.vel.y *= -elasticity;
        }
    }

public:
    ObjectSim(int x, int y, int w, int h, char* c) : Window(x,y,w,h,c) {
    }
    int getW() {return width;}
    int getH() {return height;}

    void setElasticity(double e) {
        if (e<0.0 || e>1.0) return;
        elasticity = e;
    }
    void setBorderCollision(bool b) {
        borderCollision = b;
    }
    void setForceFunctionBTWc(function<void(Circle &c1, Circle &c2)> func) {
        forceBTWc = func;
    }
    void setForceFunctionGeneral(function<void(Circle &c)> func) {
        generalF = func;
    }

    void clearSim() {
        for (unsigned int i = 0; i < circles.size(); i++) {
            delete circles[i];
        }
        for (unsigned int i = 0; i < joints.size(); i++) {
            delete joints[i];
        }
        circles.clear();
        joints.clear();
        forceBTWc = NULL;
        generalF = NULL;
        elasticity = 1;
        borderCollision = true;
    }

    void addObjects(Circle *c) {
        c->pos = {posX - width/2 + c->pos.x, posY - height/2 + c->pos.y};
        circles.push_back(c);
    }
    void addObjects(Joint2D *j) {
        joints.push_back(j);
    }

    virtual void onClick(int x, int y) override {
    }

    void fixedUpdate(float delta) {
        updatePhysics(delta);
        collisionDetection();
    }

    void updatePhysics(double delta) {
        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i]->resetAcc();
            if (generalF != NULL) {
                generalF(*circles[i]);
            }
        }
        for (unsigned int i = 0; i < joints.size(); i++) {
            joints[i]->calcForce();
        }
        if(forceBTWc != NULL) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                if (circles[i]->isLocked) continue;
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    if (circles[j]->isLocked) continue;
                    forceBTWc(*circles[i], *circles[j]);
                }
            }
        }

        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i]->update(delta);
        }
    }

    void collisionDetection() {
        for (int iter = 0; iter < 3; iter++) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                if (circles[i]->isLocked) continue;
                Circle &c1 = *circles[i];
                checkBoarder(c1);
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    if (circles[j]->isLocked) continue;
                    //Collision Happens
                    float d = circles[j]->pos<circles[i]->pos;
                    if (d < circles[i]->r + circles[j]->r) {
                        //Resolve overlap
                        V2 dist = circles[j]->pos - circles[i]->pos;
                        double angle = atan2(dist.y, dist.x);
                        V2 off = {cos(angle) * (circles[i]->r + circles[j]->r - dist.mag()),
                                  sin(angle) * (circles[i]->r + circles[j]->r - dist.mag())};
                        circles[i]->pos -= (off*0.5);
                        circles[j]->pos += (off*0.5);
                        //Resolve Momentum
                        //Normals
                        V2 n = dist*(1.0/d);
                        double relV = (circles[j]->vel.x - circles[i]->vel.x) * n.x + (circles[j]->vel.y - circles[i]->vel.y) * n.y;
                        double imp = 2 * relV / (1.0/circles[i]->mass + 1.0/circles[j]->mass) * elasticity;
                        circles[i]->vel.x += imp/circles[i]->mass * n.x;
                        circles[i]->vel.y += imp/circles[i]->mass * n.y;
                        circles[j]->vel.x -= imp/circles[j]->mass * n.x;
                        circles[j]->vel.y -= imp/circles[j]->mass * n.y;
                    }
                }
            }
        }
    }

    void draw() override {
        Window::draw();
        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i]->draw();
        }
        for (unsigned int i = 0; i < joints.size(); i++) {
            joints[i]->draw();
        }
    }
};

class ParticleSim : public Window {
private:
public:
};
