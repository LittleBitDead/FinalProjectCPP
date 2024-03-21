#include <graphics.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <functional>

#define FIXED_DELTA 0.001

using namespace std;

struct V2 {
public:
    //For precision using double
    double x, y;
    V2(double x, double y) : x(x), y(y) {}

    V2 operator+(const V2 &other) const {
        return V2(x + other.x, y + other.y);
    }
    void operator+=(const V2 &other) {
        x += other.x;
        y += other.y;
    }
    V2 operator-(const V2 &other) const {
        return V2(x - other.x, y - other.y);
    }
    void operator-=(const V2 &other) {
        x -= other.x;
        y -= other.y;
    }
    V2 operator*(const double scalar) const {
        return V2(x * scalar, y * scalar);
    }
    void operator*=(const double scalar) {
        x *= scalar;
        y *= scalar;
    }

    //Distance Op
    double operator<(const V2 &other) const {
        return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
    }

    double mag() const {
        return sqrt(x*x + y*y);
    }

    double dot(const V2 &other) const {
        return x * other.x + y * other.y;
    }

};

struct PhysicsObj2D {
    float mass = 1;
    float charge = 0;
    V2 pos = V2(0,0);
    V2 vel = V2(0,0);
    V2 acc = V2(0,0);
    virtual void draw() = 0;
};

struct Circle : public PhysicsObj2D {
    float r;
    Circle(double _x, double _y, double _r) {
        pos.x = _x;
        pos.y = _y;
        r = _r;
    }
    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        if (charge < 0)
            setfillstyle(SOLID_FILL, colors::LIGHTBLUE);
        else if (charge > 0)
            setfillstyle(SOLID_FILL, colors::LIGHTRED);
        else
            setfillstyle(SOLID_FILL, colors::LIGHTGRAY);
        fillellipse(pos.x, pos.y, r, r);
    }
};

struct Joint2D {
    V2 pos;
    V2 *objPos;
    Joint2D(double x, double y, V2 *refPos) : pos(x,y), objPos(refPos) {}
    virtual void draw() = 0;
};

struct SpringJoint: public Joint2D {
private:
    const int SEGMENTS = 20;
public:
    double springC;
    double natLength;
    double curLength;
    SpringJoint(int x, int y, V2 *refPos, double c, double l) :
        Joint2D(x,y, refPos), springC(c), natLength(l)
    {
        curLength = pos<*objPos;
    }
    double calcForce() {

    }
    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        V2 temp = *objPos - pos;
        double mag = temp.mag();

        V2 normal(-temp.y/mag, temp.x/mag);
        temp = temp * (1.0/SEGMENTS);
        V2 cur = pos;
        V2 last = pos;
        int swap = 1;
        for (int i = 0; i < SEGMENTS - 1; i++) {
            cur = pos + temp * (i + 1) + normal * 20 * swap;
            line(last.x, last.y, cur.x, cur.y);
            last = cur;
            swap *= -1;
        }
        line(last.x, last.y, objPos->x, objPos->y);
    }

};

struct RigidJoint: public Joint2D {
    V2 *objPos;
};

class Button {
private:
    int posX, posY;
    int width, height;
    function<void()> onClick;
public:
    Button(int x, int y, int w, int h, string str, function<void()> onClickF) {
    }

    bool containsPoint(int x, int y) {
        if (x < posX - width/2 || x > posX + width/2 || y < posY - height/2 || y > posY + height/2) {
            return false;
        }
        return true;
    }
    void click() {
        if (onClick != NULL) onClick();
    }
};

class Window {
private:
public:
};
class ObjectSim : public Window {
private:
public:
};
class ParticleSim : public Window {
private:
public:
};

int main() {
    initwindow(500,500, "Physics Simulation Window", true, true);
    setbkcolor(colors::WHITE);
    setcolor(colors::BLACK);
    Window optionsWin;
    ObjectSim simulationWin;

    //Timings
    auto currentTime = chrono::steady_clock::now();
    auto nextTime = currentTime + chrono::duration<double>(FIXED_DELTA);
    //BufferSwapper
    int page = 0;

    int x = 250;
    Circle c(x, 250, 100);
    SpringJoint s(0, 0, &c.pos, 10, 20);


    while (true) {
        currentTime = chrono::steady_clock::now();
        //Swap
        setvisualpage(page);
        setactivepage(1-page);
        cleardevice();


        //Physics Time Step
        if (currentTime > nextTime) {

            nextTime = currentTime + chrono::duration<double>(FIXED_DELTA);
        }
        //Draw
        c.draw();
        s.draw();
        c.pos.x = mousex();
        c.pos.y = mousey();

        delay(1);
        //Update next page
        page = 1-page;
    }
    getch();
    return 0;
}
