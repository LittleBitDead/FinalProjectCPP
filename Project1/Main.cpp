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

double const GRAVITY = 1000;
double const GRAVITATIONAL_CONST = 10000;
double const ELECTROSTATIC_CONST = 10000;
double const INELASTIC_CONST = 0.8;

double userGravDown = 1000;
double userGravConst = 10000;
double userElctroStaticConst = 10000;
double userInelastic = 0.8;


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

    char *msg1 = "Welcome to the physics sim!";
    char *msg2 = "Press the buttons to";
    char *msg3 = "start a simulation, reset";
    char *msg4 = "and adjut parameters.";
    char *msg5 = "CLICK and DRAG to move objects";

    //Create Buttons
    char* text = "Gravity";
    int bp = 10;
    int w = 70, h = 20;
    int startX = w;
    int startY = 90;
    Button *gravityB = new Button(startX,startY,w,h,"Gravity",
        [&p, &msg1, &msg2, &msg3, &msg4]() {
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
            msg1 = "Gravity!";
            msg2 = "Using Newtons Gravity";
            msg3 = "Law: F = G(mM)/(r^2)";
            msg4 = "Objects pull together";

        }
    );

    text = "Elastic";
    startX += w + bp;
    Button *elasticB = new Button(startX,startY,w,h,text,
        [&p, &msg1, &msg2, &msg3, &msg4]() {
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
            msg1 = "Elasticity!";
            msg2 = "Objects do not lose";
            msg3 = "any energy and";
            msg4 = "bounce forever";
        }
    );

    text = "Inelastic";
    startX += w + bp;
    Button *inelasticB = new Button(startX,startY,w,h,text,
        [&p, &msg1, &msg2, &msg3, &msg4]() {
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
            msg1 = "Inelastic!";
            msg2 = "Objects lose energy";
            msg3 = "and eventually";
            msg4 = "come to a stop";
        }
    );

    text = "Pendulum";
    startX = w;
    startY += h * 2 + bp;
    Button *pendulumB = new Button(startX,startY,w,h,text,
    [&p, &msg1, &msg2, &msg3, &msg4]() {
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
        Circle *c3 = new Circle(p.getWidth()/4 * 3, sy + penLen/5, 5, 15, 0);
        Circle *c4 = new Circle(p.getWidth()/4 * 3, sy + penLen/5 * 2, 5, 15, 0);

        c1->vel.x = 150;
        c2->vel.x = 150;
        c3->vel.x = 150;
        c4->vel.x = 550;

        HingeJoint* j1 = new HingeJoint(a1, c1, penLen);
        HingeJoint* j2 = new HingeJoint(a2, c2, penLen*2/3);
        HingeJoint* j3 = new HingeJoint(a3, c3, penLen/5);
        RigidJoint* j4 = new RigidJoint(c3, c4, penLen/5);

        p.addObjects(a1);
        p.addObjects(a2);
        p.addObjects(a3);

        p.addObjects(c1);
        p.addObjects(c2);
        p.addObjects(c3);
        p.addObjects(c4);

        p.addObjects(j1);
        p.addObjects(j2);
        p.addObjects(j3);
        p.addObjects(j4);

        p.setForceFunctionGeneral(generalDownGravity);

        msg1 = "Pendulums!";
        msg2 = "The frequency of the";
        msg3 = "depends on: length, gravity";
        msg4 = "but not initial energy";
        }
    );

    text = "Orbital";
    startX += w + bp;
    Button *orbitalB = new Button(startX,startY,w,h,text,
    [&p, &msg1, &msg2, &msg3, &msg4]() {
        cout << "Clearing Previous Sim!" << endl;
        p.clearSim();
        p.setBorderCollision(false);
        cout << "Starting Orbital Sim!" << endl;

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
        msg1 = "Kepler!";
        msg2 = "For any time interval";
        msg3 = "the amount of area covered";
        msg4 = "is the same throughout the orbit";
    }
    );

    text = "Harmonic";
    startX += w + bp;
    Button *harmonicB = new Button(startX,startY,w,h,text,
    [&p, &msg1, &msg2, &msg3, &msg4]() {
        cout << "Clearing Previous Sim!" << endl;
        p.clearSim();
        p.setElasticity(userInelastic);
        cout << "Starting Harmonic Sim!" << endl;
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
        msg1 = "Springs!";
        msg2 = "Using hooke's law";
        msg3 = "F = -kx, we can simulate ";
        msg4 = "harmonic/soft body systems";
        }
    );

    text = "Electro";
    startX = w;
    startY += h * 2 + bp;
    Button *electrostaticB = new Button(startX,startY,w,h,text,
    [&p, &msg1, &msg2, &msg3, &msg4]() {
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
            msg1 = "Electro!";
            msg2 = "Similar to gravity";
            msg3 = "F = kqQ/(r^2), Opposites attract";
            msg4 = "likes repel";
        }
    );

    text = "Soft/Ridig";
    startX += w + bp;
    Button *softB = new Button(startX,startY,w,h,text,
    [&p, &msg1, &msg2, &msg3, &msg4]() {
        cout << "Clearing Previous Sim!" << endl;
            p.clearSim();
            p.setElasticity(userInelastic);
            cout << "Starting Soft/Ridig Body Sim!" << endl;

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
            msg1 = "Bodies!";
            msg2 = "Using rigid arms and springs";
            msg3 = "We can put everything together";
            msg4 = "to simulate cool physics bodies";
        }
    );

    text = "RESET";
    startX += w + bp;
    Button *resetB = new Button(startX,startY,w,h,text,
    [&p, &msg1, &msg2, &msg3, &msg4]() {
            p.clearSim();
            cout << "RESET!" << endl;
            userGravDown = GRAVITY;
            userGravConst = GRAVITATIONAL_CONST;
            userElctroStaticConst = ELECTROSTATIC_CONST;
            userInelastic = INELASTIC_CONST;
            msg1 = "Welcome to the physics sim!";
            msg2 = "Press the buttons to";
            msg3 = "start a simulation, reset";
            msg4 = "and adjut parameters.";
        }
    );

    //Add Buttons
    win.addButton(gravityB);
    win.addButton(elasticB);
    win.addButton(inelasticB);
    win.addButton(pendulumB);;
    win.addButton(orbitalB);
    win.addButton(harmonicB);
    win.addButton(electrostaticB);
    win.addButton(softB);
    win.addButton(resetB);

    //Add Input
    char* hi = "Gravity";
    startY = 90;
    InputBox *in1 = new InputBox(win.getWidth() / 2, startY, w, h, hi, &userGravDown);
    hi = "Grav Const";
    startY += 50;
    InputBox *in2 = new InputBox(win.getWidth() / 2, startY, w, h, hi, &userGravConst);
    hi = "Electro";
    startY += 50;
    InputBox *in3 = new InputBox(win.getWidth() / 2, startY, w, h, hi, &userElctroStaticConst);
    hi = "Inelastic";
    startY += 50;
    InputBox *in4 = new InputBox(win.getWidth() / 2, startY, w, h, hi, &userInelastic);

    win.addButton(in1);
    win.addButton(in2);
    win.addButton(in3);
    win.addButton(in4);


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
        settextjustify(LEFT_TEXT, VCENTER_TEXT);
        outtextxy(width - 230, height-200, msg1);
        outtextxy(width - 230, height-170, msg2);
        outtextxy(width - 230, height-140, msg3);
        outtextxy(width - 230, height-110, msg4);
        outtextxy(width - 230, height-80, msg5);

        //Check for user input
        int x,y;
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (x != -1 && y != -1) {
            cout << "CLICK" << endl;
            win.onClick(x, y);
            p.onClick(x,y);
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
