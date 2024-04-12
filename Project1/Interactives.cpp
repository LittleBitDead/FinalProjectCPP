#include <graphics.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <functional>
#include <vector>
#include <conio.h>
#include <thread>
#include <cstdlib>
#include <cstdio>


#include "V2.cpp"
#include "PhysicsObjects.cpp"

#define ESCAPE_CHAR 27
#define MOUSE_LMB_DOWN 513
#define ENTER_CHAR 13
#define PI 3.14159265358979323846

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
    bool interacting = false;
    char *text;
    char buffer[20];
    char *ptr = buffer;
    double *data;

    void userInput() {
        cout << "THREAD START" << endl;
        lock = true;
        interacting = true;
        ostringstream stream;
        //Clear
        while (kbhit()) {
            getch();
        }
        while (true) {
            if (kbhit()) {
                char k = getch();
                switch ((int)k) {
                    case ESCAPE_CHAR: {
                        lock = false;
                        interacting = false;
                        return;
                    }
                    case ENTER_CHAR: {
                        goto FINISH;
                    }
                }
                stream << k;
                strncpy(buffer, stream.str().c_str(), sizeof(buffer) - 1);
            }
        }
        FINISH:
        const char *in = stream.str().c_str();
        char* endPtr;
        double result = strtod(in, &endPtr);
        if (endPtr != in && *endPtr == '\0') {
            std::cout << "Conversion successful. Double value: " << result << std::endl;
            *data = result;
        } else {
            snprintf(buffer, sizeof(buffer), "%.2f", *data);
            std::cout << "Conversion failed. Invalid input." << std::endl;
        }
        interacting = false;
        lock = false;
        cout << "THREAD END" << endl;
    }

public:
    InputBox(int x, int y, int w, int h, char* str, double *d) {
        posX = x;
        posY = y;
        width = w;
        height = h;
        text = str;
        data = d;
        snprintf(buffer, sizeof(buffer), "%.2f", *data);
    }

    void onClick(int x, int y) override {
        if (clickCheck(x,y)) {
            if (lock == false) {
                std::thread getInput(&InputBox::userInput, this);
                getInput.detach();
            }
        }
    }

    virtual void draw() override {
        if (!lock)
            snprintf(buffer, sizeof(buffer), "%.2f", *data);
        setcolor(colors::BLACK);
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
        if (interacting) {
            setfillstyle(BKSLASH_FILL, RED);
        } else {
            setfillstyle(SOLID_FILL, DARKGRAY);
        }
        setbkcolor(DARKGRAY);
        rectangle(posX - width/2, posY - height,
                  posX + width/2, posY + height/2);
        floodfill(posX, posY, BLACK);

        setcolor(WHITE);
        settextjustify(CENTER_TEXT, VCENTER_TEXT);
        outtextxy(posX, posY, text);
        settextjustify(LEFT_TEXT, VCENTER_TEXT);
        outtextxy(posX + 50, posY, buffer);
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
    static bool lock;
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

    void drag(Circle *circle) {
        cout << "DRAG START" << endl;
        lock = true;
        POINT cursor;
        while (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            GetCursorPos(&cursor);
            circle->vel.x = 0;
            circle->vel.y = 0;
            circle->pos.x = cursor.x;
            circle->pos.y = cursor.y - 25;
        }

        lock = false;
        cout << "DRAG END" << endl;
    }

public:
    ObjectSim(int x, int y, int w, int h, char* c) : Window(x,y,w,h,c) {
    }

    void setElasticity(double &e) {
        if (e < 0.0) {
            e = 0;
            return;
        }
        if (e > 1.0) {
            e = 1;
            return;
        }
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

    const int clickSize = 10;

    virtual void onClick(int x, int y) override {
        const Circle clicker(x,y, clickSize, -1, 0);
        for (unsigned int i = 0; i < circles.size(); i++) {
            if (lock == false) {
                float d = clicker.pos<circles[i]->pos;
                if (d < circles[i]->r + clicker.r) {
                    auto lambdaFunc = [&] (int i) {
                        drag(circles[i]);
                    };
                    std::thread dragT(lambdaFunc, i);
                    dragT.detach();
                    break;
                }
            }
        }
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

        if(forceBTWc != NULL) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                if (circles[i]->isLocked) continue;
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    if (circles[j]->isLocked) continue;
                    forceBTWc(*circles[i], *circles[j]);
                }
            }
        }

        for (unsigned int i = 0; i < joints.size(); i++) {
            joints[i]->calcForce();
        }

        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i]->update(delta);
        }
    }

    void collisionDetection() {
        for (int iter = 0; iter < 2; iter++) {
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

bool ObjectSim::lock = false;
