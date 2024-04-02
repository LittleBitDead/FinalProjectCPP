#include <graphics.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <functional>
#include <vector>

#define FIXED_DELTA 0.005
float GRAVITY = 10;
float GRAVITATIONAL_CONST = 10000;

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
    bool isLocked = false;
    bool infoMode = false;
    virtual void draw() = 0;
    void update(const double D) {
        if (isLocked) return;
        vel += acc * D;
        pos += vel * D;
    };
    void inline resetAcc() {
        acc.x = 0;
        acc.y = 0;
    }
    void setStatic(bool b) {
        isLocked = b;
    }
};

struct Circle : public PhysicsObj2D {
    float r;
    Circle(double _x, double _y, double _r, float m, float c) {
        pos.x = _x;
        pos.y = _y;
        r = _r;
        mass = m;
        charge = c;
    }
    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        setcolor(colors::BLACK);
        if (charge < 0)
            setfillstyle(SOLID_FILL, colors::LIGHTBLUE);
        else if (charge > 0)
            setfillstyle(SOLID_FILL, colors::LIGHTRED);
        else
            setfillstyle(SOLID_FILL, colors::LIGHTGRAY);
        fillellipse(pos.x, pos.y, r, r);

        if (infoMode) {
            setcolor(colors::RED);
            line(pos.x, pos.y, pos.x + vel.x, pos.y + vel.y);
            setcolor(colors::BLUE);
            line(pos.x, pos.y, pos.x + acc.x, pos.y + acc.y);
        }
    }
};

struct Joint2D {
    PhysicsObj2D *from;
    PhysicsObj2D *to;

    Joint2D(PhysicsObj2D *from, PhysicsObj2D *to) : from(from), to(to) {}
    virtual void draw() = 0;
};

struct SpringJoint: public Joint2D {
private:
    const int SEGMENTS = 20;
    const int SPRING_WIDTH = 10;
public:
    double k;
    double x;

    SpringJoint(PhysicsObj2D *from, PhysicsObj2D *to, double c, double l) : Joint2D(from, to), k(c), x(l) {}

    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        setcolor(colors::BLACK);
        V2 stepDis = (to->pos - from->pos) * (1.0/SEGMENTS);
        V2 normal(-stepDis.y, stepDis.x);
        normal *= (1.0/normal.mag());

        V2 cur = from->pos;
        V2 last = from->pos;
        short swap = 1;
        for (int i = 0; i < SEGMENTS - 1; i++) {
            cur = from->pos + stepDis * (i + 1) + normal * SPRING_WIDTH * swap;
            line(last.x, last.y, cur.x, cur.y);
            last = cur;
            swap *= -1;
        }
        line(last.x, last.y, to->pos.x, to->pos.y);
    }

};

struct RigidJoint: public Joint2D {
    V2 *objPos;
};

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
    virtual void onClick() = 0;
    virtual void draw() const = 0;
};

class Button : public Element {
    int padding;
    bool hover = false;
    char *text;
    function<void()> func;

public:
    Button(int x, int y, int p, char* str, function<void()> func) : func(func) {
        posX = x;
        posY = y;
        padding = p;
        text = str;
        width = textwidth(text) + padding*4;
        height = textheight(text) + padding;
    }

    void setXY(int x, int y) {
        posX = x;
        posY = y;
    }
    int getX() { return posX; }
    int getY() { return posY; }

    bool containsPoint(int x, int y) {
        if (x < posX - width/2 || x > posX + width/2 || y < posY - height/2 || y > posY + height/2) {
            hover = false;
            return false;
        }
        hover = true;
        return true;
    }

    virtual void onClick() override {
        if (func != NULL) func();
    }

    virtual void draw() const override {
        setcolor(colors::BLACK);
        setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
        if (hover) {
            setfillstyle(SOLID_FILL, LIGHTGRAY);
            setbkcolor(LIGHTGRAY);
        } else {
            setfillstyle(SOLID_FILL, DARKGRAY);
            setbkcolor(DARKGRAY);
        }
        rectangle(posX - width/2, posY - height,
                  posX + width/2, posY + height/2);
        floodfill(posX, posY, BLACK);

        setcolor(WHITE);
        settextjustify(CENTER_TEXT, VCENTER_TEXT);
        outtextxy(posX, posY, text);
    }
};

class Window : public Element {
private:
    vector<Button*> btns;
    Button btn;


public:
    Window(int x, int y, int w, int h, char* c) : btn(x, y - h/2 + textheight(c) * 2, 5, c, NULL) {
        posX = x;
        posY = y;
        width = w;
        height = h;
    }

    void addButton(Button* btn) {

        btn->setXY(posX-width/2 + btn->getX(), posY - height/2 + btn->getY());
        btns.push_back(btn);

    }
    void handleClick(int x, int y, bool click) {

    }

    virtual void onClick() override { }

    virtual void draw() const override {
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

        for (int i = 0; i < btns.size(); i++) {
            btns[i]->draw();
        }
    }
};

class ObjectSim : public Window {
private:
    double elasticity = 1;
    bool borderCollision = true;
    vector<Circle> circles;
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
        circles.clear();
        forceBTWc = NULL;
        generalF = NULL;
        elasticity = 1;
        borderCollision = true;
    }

    void addObjects(Circle c) {
        c.pos = {posX - width/2 + c.pos.x, posY - height/2 + c.pos.y};
        circles.push_back(c);
    }

    void updatePhysics(double delta) {
        for (unsigned int i = 0; i < circles.size(); i++) {
            circles[i].resetAcc();
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
            circles[i].update(FIXED_DELTA);
        }
    }

    void collisionDetection() {
        for (int iter = 0; iter < 3; iter++) {
            for (unsigned int i = 0; i < circles.size(); i++) {
                Circle &c1 = circles[i];
                checkBoarder(c1);
                for (unsigned int j = i + 1; j < circles.size(); j++) {
                    //Collision Happens
                    float d = circles[j].pos<circles[i].pos;
                    if (d < circles[i].r + circles[j].r) {
                        //Resolve overlap
                        V2 dist = circles[j].pos - circles[i].pos;
                        double angle = atan2(dist.y, dist.x);
                        V2 off = {cos(angle) * (circles[i].r + circles[j].r - dist.x),
                                  sin(angle) * (circles[i].r + circles[j].r - dist.y)};
                        circles[i].pos -= off*0.5;
                        circles[j].pos += off*0.5;
                        //Resolve Momentum
                        //Normals
                        V2 n = dist*(1.0/d);
                        double relV = (circles[j].vel.x - circles[i].vel.x) * n.x + (circles[j].vel.y - circles[i].vel.y) * n.y;
                        double imp = 2 * relV / (1.0/circles[i].mass + 1.0/circles[j].mass) * elasticity;
                        circles[i].vel.x += imp/circles[i].mass * n.x;
                        circles[i].vel.y += imp/circles[i].mass * n.y;
                        circles[j].vel.x -= imp/circles[j].mass * n.x;
                        circles[j].vel.y -= imp/circles[j].mass * n.y;
                    }
                }
            }
        }
    }
};

class ParticleSim : public Window {
private:
public:
};

void generalDownGravity(Circle &c) {
    c.acc.y += GRAVITY;
}

void gravityFunc(Circle &c1, Circle &c2) {
    V2 delta = c2.pos - c1.pos;
    double force = GRAVITATIONAL_CONST * c1.mass * c2.mass / (delta.mag() * delta.mag());
    double angle = atan2(delta.y, delta.x);
    V2 forceV = {cos(angle) * force, sin(angle) * force};
    c1.acc += forceV * (1.0/c1.mass);
    c1.acc -= forceV * (1.0/c2.mass);

}

int main() {
    int width = 720, height = 720;
    initwindow(width,height, "Physics Simulation Window", true, true);

    int pad = 10;
    float ratio = 0.60;

    ObjectSim p(width/2, height*ratio/2, width - pad,height*ratio - pad, "PHYSICS OBJECTS SIM");
    Window w(width/2, height - height*(1.0 - ratio)/2, width - pad,height*(1.0-ratio) - pad, "OPTIONS");

    //Create Buttons
    char* text = "Gravity";
    int padding = 10;
    int startX = textwidth(text);
    int startY = 90;
    Button gravityB(startX,startY,5,"Gravity",
        [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            cout << "Starting Gravity Sim!" << endl;
            Circle c1 = {250, 250, 50, 50, 0};
            Circle c2 = {100, 250, 40, 40, 0};
            Circle c3 = {250, 100, 30, 30, 0};
            Circle c4 = {100, 100, 20, 20, 0};
            p.setForceFunctionBTWc(gravityFunc);
            p.addObjects(c1);
            p.addObjects(c2);
            p.addObjects(c3);
            p.addObjects(c4);
        }
    );

    text = "Elastic";
    startX += gravityB.getWidth()/2 + textwidth(text);
    Button elasticB(startX,startY,5,text,
        [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            p.setElasticity(0.99);
            cout << "Starting Gravity Sim!" << endl;
            //r x, y, vx, vy, m, c, ax, ay
            int radius = 15;
            int mass = 10;
            int rows = 6, cols = 5;
            int spacingX = p.getWidth() / rows;
            int spacingY = p.getHeight() / cols;
            int maxV = 1000;

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j ++) {
                    double x = p.getPosX() - p.getWidth() + spacingX * (i + 0.5);
                    double y = p.getPosY() - p.getHeight() + spacingY * (j+ 0.5);
                    Circle c = {x,y,radius, mass, 0};
                    p.addObjects(c);
                }
            }
        }
    );

    text = "Inelastic";
    startX += elasticB.getWidth()/2 + textwidth(text);
    Button inelasticB(startX,startY,5, text,
            [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            p.setElasticity(0.8);
            cout << "Starting Gravity Sim!" << endl;
            //r x, y, vx, vy, m, c, ax, ay
            int radius = 15;
            int mass = 10;
            int rows = 6, cols = 5;
            int spacingX = p.getWidth() / rows;
            int spacingY = p.getHeight() / cols;
            int maxV = 1000;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j ++) {
                    int x = p.getPosX() - p.getWidth() + spacingX * (i + 0.5);
                    int y = p.getPosY() - p.getHeight() + spacingY * (j+ 0.5);
                    Circle c = {x,y,radius, mass, 0};
                    p.addObjects(c);
                }
            }
        }
    );

    text = "Pendulum";
    startX += inelasticB.getWidth()/2 + textwidth(text);
    Button pendulumB(startX,startY,5,text, NULL);

    text = "Projectile";
    startX += pendulumB.getWidth()/2 + textwidth(text);
    Button projectileB(startX,startY,5,text, NULL);

    text = "Orbital";
    startX += projectileB.getWidth()/2 + textwidth(text);
    Button orbitalB(startX,startY,5,text, NULL);

    text = "Fluid";
    startX += orbitalB.getWidth()/2 + textwidth(text);
    Button fluidB(startX,startY,5,text, NULL);

    text = "Harmonic";
    startX += fluidB.getWidth()/2 + textwidth(text);
    Button harmonicB(startX,startY,5,text, NULL);

    text = "Electro";
    startX += harmonicB.getWidth()/2 + textwidth(text);
    Button electrostaticB(startX,startY,5,text, NULL);

    text = "Thermo";
    startX += electrostaticB.getWidth()/2 + textwidth(text);
    Button thermoB(startX,startY,5,text, NULL);

    //Add Buttons
    w.addButton(&gravityB);
    w.addButton(&elasticB);
    w.addButton(&inelasticB);
    w.addButton(&pendulumB);
    w.addButton(&projectileB);
    w.addButton(&orbitalB);
    w.addButton(&fluidB);
    w.addButton(&harmonicB);
    w.addButton(&electrostaticB);
    w.addButton(&thermoB);

    //Timings
    auto currentTime = chrono::steady_clock::now();
    auto nextTime = currentTime + chrono::duration<double>(FIXED_DELTA);
    //BufferSwapper
    int page = 0;

    while (true) {
        currentTime = chrono::steady_clock::now();
        //Swap
        setvisualpage(page);
        setactivepage(1-page);
        setbkcolor(colors::WHITE);
        cleardevice();

        //Draw
        p.draw();
        w.draw();

        //Physics Time Step
        if (currentTime > nextTime) {

            nextTime = currentTime + chrono::duration<double>(FIXED_DELTA);
        }

        //Update next page
        page = 1-page;
    }
    getch();
    return 0;
}
