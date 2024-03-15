#include <simplecpp>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <string>
#include <random>
#include "Button.cpp"
#include "Window.cpp"

using namespace std;

class InputBox : public Window {
    bool active = false;
    //Default ESCAPE key
    char escapeKey = 27;
    //Default Enter Key
    char enterKey = 13;

public:
    InputBox(int x, int y, int w, int h, string n) : Window(x,y,w,h,n) {
        label.hide();
        box.hide();
    }

    int getInput(string str) {
        active = true;

        XEvent event;
        stringstream ss;
        int out;

        label.show();
        box.show();

        while (active) {
            nextEvent(event);
            if(keyPressEvent(event)){
                cout << (int)charFromEvent(event) << endl;
                if(charFromEvent(event) == escapeKey) {
                    active = false;
                    return -1;
                }
                if(charFromEvent(event) == enterKey) {
                    ss >> out;
                    return out;
                }
                ss << charFromEvent(event);
                label.setMessage(ss.str());
            }
        }
        return -1;
    }
};

const int WIDTH = 520, HEIGHT = 520;
const int sizeW = 6;
const float GRAVITY = 5;
const float GRAVITATIONAL_CONST = 1000;
//A delta of ~0.002
//Allows the XEvent framework
//to preform drag, press and release events.
//Changing this parameter might cause the program
//To break.
const float delta = 0.002;

void generalDownGravity(Cir &c) {
    c.ay += GRAVITY;
}

void gravityFunc(Cir &c1, Cir &c2) {
    float dx = c2.x - c1.x;
    float dy = c2.y - c1.y;
    float d2 = (dx * dx + dy * dy);
    float force = (GRAVITATIONAL_CONST * c1.mass * c2.mass) / d2;
    d2 = sqrt(d2);
    float fx = (dx/d2) * force;
    float fy = (dy/d2) * force;
    c1.ax += fx / c1.mass;
    c1.ay += fy / c1.mass;
    c2.ax -= fx / c2.mass;
    c2.ay -= fy / c2.mass;
}

int main() {
    initCanvas("Simulator", WIDTH, HEIGHT);
    Window w(WIDTH/2, HEIGHT - HEIGHT/sizeW, WIDTH, HEIGHT/sizeW * 2, "Options");
    PhysicsSim p(WIDTH/2, HEIGHT/sizeW * 2, WIDTH, HEIGHT/sizeW * 4, "Physics Sim");

    beginFrame();

    //Create Buttons
    Button gravityB(40,425,70,30,"Gravity",
    [&p]() {
        cout << "Clearing Previous Sim!" << endl;
        p.clearSim();
        cout << "Starting Gravity Sim!" << endl;
        //r x, y, vx, vy, m, c, ax, ay
        Cir c1 = {35, 0, 0, 0, 0, 0, 0, 100, 0};
        Cir c2 = {25, -150, 150, -100, 100, 0, 0, 20, 1};
        Cir c3 = {10, 150, 150, 0, 10, 0, 0, 5, -1};
        p.setForceFunctionBTWc(gravityFunc);
        p.addObjects(c1);
        p.addObjects(c2);
        p.addObjects(c3);
    }
    );
    //Button elasticB(120,425,70,30,"Elastic", elastic);
    //Button inelasticB(200,425,70,30,"Inelastic", inelastic);
    //Button pendulumB(280,425,70,30,"Pendulum", pendulum);
    //Button projectileB(360,425,70,30,"Projectile", projectile);
    //Button orbitalB(40,475,70,30,"Orbital", orbital);
    //Button fluidB(120,475,70,30,"Fluid", fluid);
    //Button harmonicB(200,475,70,30,"Harmonic", harmonic);
    //Button electrostaticB(280,475,70,30,"Electro", electrostatic);
    //Button thermoB(360, 475,70,30,"Thermo", thermo);

    //Add Buttons
    w.addButton(&gravityB);
    //w.addButton(&elasticB);
    //w.addButton(&inelasticB);
    //w.addButton(&pendulumB);
    //w.addButton(&projectileB);
    //w.addButton(&orbitalB);
    //w.addButton(&fluidB);
    //w.addButton(&harmonicB);
    //w.addButton(&electrostaticB);
    //w.addButton(&thermoB);
    endFrame();

    //Mouse
    XEvent event;

    event.xmotion.x = 0;
    event.xmotion.y = 0;

    nextEvent(event);
    while (true) {
        if (checkEvent(event)) {
            if (mouseButtonReleaseEvent(event)) {
                w.handleClick(event.xmotion.x, event.xmotion.y);
                p.handleIteract(event.xmotion.x, event.xmotion.y);
                cout << event.xmotion.x << " PRESS " << event.xmotion.y<< endl;
            }
        }
        p.updatePhysics(delta);
        p.collisionDetection();
        p.updateGraphics();
        //This makes sure that
        //The XEvents are properly updated
        //And is the physics time step
        wait(delta);
    }

    getClick();
}
