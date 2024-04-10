#include <graphics.h>
#include <iostream>
#include <math.h>
#include <chrono>
#include <functional>
#include <vector>
#include <random>

#include "V2.cpp"
#include "PhysicsObjects.cpp"
#include "Interactives.cpp"

#define FIXED_DELTA 0.003

float const GRAVITY = 1000;
float const GRAVITATIONAL_CONST = 10000;
float const INELASTIC_CONST = 0.8;

float userGravDown = 1000;
float userGravConst = 10000;
float userElctroStaticConst = 10000;
float userInelastic = 0.8;


using namespace std;

void generalDownGravity(Circle &c) {
    c.acc.y += userGravDown;
}

void gravityFunc(Circle &c1, Circle &c2) {
    V2 delta = c2.pos - c1.pos;
    double force = userGravConst * c1.mass * c2.mass / (delta.mag() * delta.mag());
    double angle = atan2(delta.y, delta.x);
    V2 forceV = {cos(angle) * force, sin(angle) * force};
    c1.acc += forceV * (1.0/c1.mass);
    c2.acc -= forceV * (1.0/c2.mass);
}

void electroStaticFunc(Circle &c1, Circle &c2) {
    V2 delta = c2.pos - c1.pos;
    double force = userElctroStaticConst * c1.charge * c2.charge / (delta.mag() * delta.mag());
    double angle = atan2(delta.y, delta.x);
    V2 forceV = {cos(angle) * force, sin(angle) * force};
    c1.acc -= forceV * (1.0/c1.mass);
    c2.acc += forceV * (1.0/c2.mass);
}

int main() {
    int width = 720, height = 720;
    initwindow(width,height, "Physics Simulation Window", true, true);

    int pad = 10;
    float ratio = 0.60;

    ObjectSim p(width/2, height*ratio/2, width - pad,height*ratio - pad, "PHYSICS OBJECTS SIM");
    Window win(width/2, height - height*(1.0 - ratio)/2, width - pad,height*(1.0-ratio) - pad, "OPTIONS");

    //Create Buttons
    char* text = "Gravity";
    int bp = 10;
    int w = 70, h = 20;
    int startX = w;
    int startY = 90;
    Button *gravityB = new Button(startX,startY,w,h,"Gravity",
        [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            cout << "Starting Gravity Sim!" << endl;
            Circle *c1 = new Circle(250, 250, 50, 50, 10);
            Circle *c2 = new Circle(100, 250, 40, 40, -10);
            Circle *c3 = new Circle(250, 100, 30, 30, 0);
            Circle *c4 = new Circle(100, 100, 20, 20, 0);

            p.addObjects(c1);
            p.addObjects(c2);
            p.addObjects(c3);
            p.addObjects(c4);
            p.setForceFunctionBTWc(gravityFunc);
        }
    );

    text = "Elastic";
    startX += w + bp;
    Button *elasticB = new Button(startX,startY,w,h,text,
        [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            cout << "Starting Elastic Sim!" << endl;
            //r x, y, vx, vy, m, c, ax, ay
            int radius = 15;
            int mass = 10;
            int rows = 6, cols = 5;
            int spacingX = p.getWidth() / rows;
            int spacingY = p.getHeight() / cols;
            int maxV = 1000;

            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> dis(-maxV, maxV);

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j ++) {
                    double x = spacingX * (i + 0.5);
                    double y = spacingY * (j+ 0.5);
                    Circle *c = new Circle(x, y, radius, mass, 0);
                    c->vel = {dis(gen), dis(gen)};
                    p.addObjects(c);
                }
            }

        }
    );

    text = "Inelastic";
    startX += w + bp;
    Button *inelasticB = new Button(startX,startY,w,h,text,
        [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            p.setElasticity(userInelastic);
            cout << "Starting Inelastic Sim!" << endl;
            //r x, y, vx, vy, m, c, ax, ay
            int radius = 15;
            int mass = 10;
            int rows = 6, cols = 5;
            int spacingX = p.getWidth() / rows;
            int spacingY = p.getHeight() / cols;
            int maxV = 1000;

            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> dis(-maxV, maxV);

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j ++) {
                    double x = spacingX * (i + 0.5);
                    double y = spacingY * (j+ 0.5);
                    Circle *c = new Circle(x, y, radius, mass, 0);
                    c->vel = {dis(gen), dis(gen)};
                    p.addObjects(c);
                }
            }
        }
    );

    text = "Pendulum";
    startX = w;
    startY += h * 2 + bp;
    Button *pendulumB = new Button(startX,startY,w,h,text,
    [&p]() {
        cout << "Clearing Previous Sim!" << endl;
        p.clearSim();
        p.setElasticity(userInelastic);
        cout << "Starting Pendulum Sim!" << endl;
        int penLen = 250;
        int sy = 70;

        Circle *a1 = new Circle(p.getWidth()/4 * 1, sy, 5, 5, 0);
        Circle *a2 = new Circle(p.getWidth()/4 * 2, sy, 5, 5, 0);
        Circle *a3 = new Circle(p.getWidth()/4 * 3, sy, 5, 5, 0);

        Circle *c1 = new Circle(p.getWidth()/4 * 1, sy + penLen, 15, 15, 0);
        Circle *c2 = new Circle(p.getWidth()/4 * 2, sy + penLen*2/3, 15, 15, 0);
        Circle *c3 = new Circle(p.getWidth()/4 * 3, sy + penLen/3, 15, 15, 0);

        c1->vel.x = 150;
        c2->vel.x = 150;
        c3->vel.x = 150;

        HingeJoint* j1 = new HingeJoint(a1, c1, penLen);
        HingeJoint* j2 = new HingeJoint(a2, c2, penLen*2/3);
        HingeJoint* j3 = new HingeJoint(a3, c3, penLen/3);
        HingeJoint* j3 = new HingeJoint(a3, c3, penLen/3);

        p.addObjects(a1);
        p.addObjects(a2);
        p.addObjects(a3);

        p.addObjects(c1);
        p.addObjects(c2);
        p.addObjects(c3);

        p.addObjects(j1);
        p.addObjects(j2);
        p.addObjects(j3);

        p.setForceFunctionGeneral(generalDownGravity);
        }
    );

    text = "Projectile";
    startX += w + bp;
    Button *projectileB = new Button(startX,startY,w,h,text, NULL);

    text = "Orbital";
    startX += w + bp;
    Button *orbitalB = new Button(startX,startY,w,h,text, [&p]() {
        cout << "Clearing Previous Sim!" << endl;
        p.clearSim();
        p.setBorderCollision(false);
        cout << "Starting Gravity Sim!" << endl;

        int midX = p.getWidth()/2;
        int midY = p.getHeight()/2;
        int mass = 5000;
        int radius = 25;
        int massSmol = 5;
        int radiusSmol = 5;

        Circle *c1 = new Circle(midX, midY, radius, mass, 0);
        Circle *c2= new Circle(midX + radius + radiusSmol * 10, midY, radiusSmol, massSmol, 0);
        Circle *c3= new Circle(midX + radius + radiusSmol * 20, midY, radiusSmol, massSmol, 0);

        c2->vel.y = 800;
        c3->vel.y = 750;

        p.addObjects(c1);
        p.addObjects(c2);
        p.addObjects(c3);
        p.setForceFunctionBTWc(gravityFunc);
    }
    );

    text = "Fluid";
    startX = w;
    startY += h * 2 + bp;
    Button *fluidB = new Button(startX,startY,w,h,text, NULL);

    text = "Harmonic";
    startX += w + bp;
    Button *harmonicB = new Button(startX,startY,w,h,text, [&p]() {
        cout << "Clearing Previous Sim!" << endl;
        p.clearSim();
        p.setElasticity(userInelastic);
        cout << "Starting Pendulum Sim!" << endl;
        int penLen = 50;
        int sy = 200;

        Circle *c1 = new Circle(p.getWidth()/4, sy, 20, 20, 0);
        Circle *c2 = new Circle(p.getWidth() * 2 / 4, sy + penLen, 20, 20, 10);
        Circle *c3 = new Circle(p.getWidth() * 3 / 4, sy + penLen * 2, 20, 20, -10);

        Circle *a1 = new Circle(p.getWidth()/4, sy - penLen, 5, 5, 0);
        Circle *a2 = new Circle(p.getWidth() * 2 / 4, sy - penLen, 5, 5, 0);
        Circle *a3 = new Circle(p.getWidth() * 3 / 4, sy - penLen, 5, 5, 0);

        a1->setStatic(true);
        a2->setStatic(true);
        a3->setStatic(true);

        SpringJoint* j1 = new SpringJoint(c1,a1, 500.0, penLen, 0);
        SpringJoint* j2 = new SpringJoint(c2,a2, 500.0, penLen, 0);
        SpringJoint* j3 = new SpringJoint(c3,a3, 500.0, penLen, 0);
        SpringJoint* j4 = new SpringJoint(c2,c1, 500.0, penLen, 0.5);

        p.addObjects(c1);
        p.addObjects(c2);
        p.addObjects(c3);

        p.addObjects(a1);
        p.addObjects(a2);
        p.addObjects(a3);

        p.addObjects(j1);
        p.addObjects(j2);
        p.addObjects(j3);
        p.addObjects(j4);
        p.setForceFunctionGeneral(generalDownGravity);
        }
    );

    text = "Electro";
    startX += w + bp;
    Button *electrostaticB = new Button(startX,startY,w,h,text, [&p]() {
            cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            p.setElasticity(userInelastic);
            cout << "Starting Electro Sim!" << endl;
            Circle *c1 = new Circle(250, 250, 20, 30, -500);
            Circle *c2 = new Circle(100, 250, 20, 30, -100);
            Circle *c3 = new Circle(250, 100, 20, 30, -100);
            Circle *c4 = new Circle(100, 100, 20, 30, 200);
            Circle *c5 = new Circle(400, 200, 20, 30, 200);
            Circle *c6 = new Circle(300, 300, 20, 30, 200);

            p.addObjects(c1);
            p.addObjects(c2);
            p.addObjects(c3);
            p.addObjects(c4);
            p.addObjects(c5);
            p.addObjects(c6);
            p.setForceFunctionBTWc(electroStaticFunc);
        }
    );

    text = "Thermo";
    startX = w;
    startY += h * 2 + bp;
    Button *thermoB = new Button(startX,startY,w,h,text, NULL);

    text = "Soft/Ridig";
    startX += w + bp;
    Button *softB = new Button(startX,startY,w,h,text,
    [&p]() {
        cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            p.setElasticity(userInelastic);
            cout << "Starting Soft Body Sim!" << endl;

            int midX = p.getWidth()/4;
            int midY = p.getHeight()/2;
            int offset = 50;

            int springF = 10000;
            int springD = 1.3;

            Circle *c1 = new Circle(midX - offset, midY - offset, 15, 30, 0);
            Circle *c2 = new Circle(midX - offset, midY + offset, 15, 30, 0);
            Circle *c3 = new Circle(midX + offset, midY - offset, 15, 30, 0);
            Circle *c4 = new Circle(midX + offset, midY + offset, 15, 30, 0);
            Circle *c5 = new Circle(midX, midY, 15, 30, 0);

            midX *= 3;
            Circle *c6 = new Circle(midX - offset, midY - offset, 15, 30, 0);
            Circle *c7 = new Circle(midX - offset, midY + offset, 15, 30, 0);
            Circle *c8 = new Circle(midX + offset, midY - offset, 15, 30, 0);
            Circle *c9 = new Circle(midX + offset, midY + offset, 15, 30, 0);
            Circle *c10 = new Circle(midX, midY, 15, 30, 0);

            SpringJoint* j1 = new SpringJoint(c1,c2, springF, offset * 2, springD);
            SpringJoint* j2 = new SpringJoint(c2,c4, springF, offset * 2, springD);
            SpringJoint* j3 = new SpringJoint(c3,c1, springF, offset * 2, springD);
            SpringJoint* j4 = new SpringJoint(c4,c3, springF, offset * 2, springD);
            SpringJoint* j5 = new SpringJoint(c1,c5, springF, offset * 2, springD);
            SpringJoint* j6 = new SpringJoint(c2,c5, springF, offset * 2, springD);
            SpringJoint* j7 = new SpringJoint(c3,c5, springF, offset * 2, springD);
            SpringJoint* j8 = new SpringJoint(c4,c5, springF, offset * 2, springD);

            RigidJoint* j11 = new RigidJoint(c6,c7, 100);
            RigidJoint* j12 = new RigidJoint(c7,c9, 100);
            RigidJoint* j13 = new RigidJoint(c8,c6, 100);
            RigidJoint* j14 = new RigidJoint(c9,c8, 100);
            RigidJoint* j15 = new RigidJoint(c6,c10,70.7106781187);
            RigidJoint* j16 = new RigidJoint(c7,c10,70.7106781187);
            RigidJoint* j17 = new RigidJoint(c8,c10,70.7106781187);
            RigidJoint* j18 = new RigidJoint(c9,c10,70.7106781187);

            p.addObjects(c1);
            p.addObjects(c2);
            p.addObjects(c3);
            p.addObjects(c4);
            p.addObjects(c5);
            p.addObjects(c6);
            p.addObjects(c7);
            p.addObjects(c8);
            p.addObjects(c9);
            p.addObjects(c10);

            p.addObjects(j1);
            p.addObjects(j2);
            p.addObjects(j3);
            p.addObjects(j4);
            p.addObjects(j5);
            p.addObjects(j6);
            p.addObjects(j7);
            p.addObjects(j8);
            p.addObjects(j11);
            p.addObjects(j12);
            p.addObjects(j13);
            p.addObjects(j14);
            p.addObjects(j15);
            p.addObjects(j16);
            p.addObjects(j17);
            p.addObjects(j18);
            p.setForceFunctionGeneral(generalDownGravity);
        }
    );

    //Add Buttons
    win.addButton(gravityB);
    win.addButton(elasticB);
    win.addButton(inelasticB);
    win.addButton(pendulumB);
    win.addButton(projectileB);
    win.addButton(orbitalB);
    win.addButton(fluidB);
    win.addButton(harmonicB);
    win.addButton(electrostaticB);
    win.addButton(thermoB);
    win.addButton(softB);

    char* hi = "Gravity";
    startY = 90;
    InputBox *in = new InputBox(win.getWidth() / 2 + 50, startY, w, h, hi);
    win.addButton(in);


    //Timings
    auto currentTime = chrono::steady_clock::now();
    auto nextTime = currentTime + chrono::duration<double>(FIXED_DELTA);
    //BufferSwapper
    int page = 0;

    cout << WM_LBUTTONDOWN << endl;

    while (true) {
        currentTime = chrono::steady_clock::now();
        //Swap
        setvisualpage(page);
        setactivepage(1-page);
        setbkcolor(colors::WHITE);
        cleardevice();

        //Draw
        p.draw();
        win.draw();

        //Check for user input
        int x,y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (x != -1 && y != -1) {
            cout << "CLICK" << endl;
            win.onClick(x, y);
            clearmouseclick(WM_LBUTTONDOWN);
        }

        //Physics Time Step
        if (currentTime > nextTime) {
            p.fixedUpdate(FIXED_DELTA);
            nextTime = currentTime + chrono::duration<double>(FIXED_DELTA);
        }

        //Update next page
        page = 1-page;
    }
    getch();
    return 0;
}
