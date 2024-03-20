#include <simplecpp>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <string>
#include <random>
#include "Button.cpp"
#include "Window.cpp"

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
const double GRAVITY = 5;
const double GRAVITATIONAL_CONST = 10000;
const double DELTA = 0.005;

void generalDownGravity(Cir &c) {
    c.ay += GRAVITY;
    cout << "GRAV DOWN" << endl;
    cout << c.x << " " << c.y << endl;
    cout << c.vx << " " << c.vy << endl;
    cout << c.ax << " " << c.ay << endl;
}

void gravityFunc(Cir &c1, Cir &c2) {
    double dx = c2.x - c1.x;
    double dy = c2.y - c1.y;
    double force = GRAVITATIONAL_CONST * c1.mass * c2.mass / (dx * dx + dy * dy);
    double angle = atan2(dy, dx);
    double fx = cos(angle) * force;
    double fy = sin(angle) * force;
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
            Cir c1 = {35, 0, 0, 0, 0, 0, 0, 100, 0};
            Cir c2 = {25, -100, 100, 0, 0, 0, 0, 70, 50};
            Cir c3 = {16, 100, 100, 0, 0, 0, 0, 25, -50};
            Cir c4 = {16, 150, 100, 0, 0, 0, 0, 5, 0};
            p.setForceFunctionBTWc(gravityFunc);
            p.addObjects(c1);
            p.addObjects(c2);
            p.addObjects(c3);
            p.addObjects(c4);
        }
    );
    Button elasticB(120,425,70,30,"Elastic",
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

            beginFrame();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j ++) {
                    int x = p.getPosX() - p.getWidth() + spacingX * (i + 0.5);
                    int y = p.getPosY() - p.getHeight() + spacingY * (j+ 0.5);
                    Cir c = {radius, x, y,randuv(-maxV,maxV),randuv(-maxV,maxV),0,0,mass,0};
                    p.addObjects(c);
                }
            }
            endFrame();
        }
    );
    Button inelasticB(200,425,70,30,"Inelastic",
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
            beginFrame();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j ++) {
                    int x = p.getPosX() - p.getWidth() + spacingX * (i + 0.5);
                    int y = p.getPosY() - p.getHeight() + spacingY * (j+ 0.5);
                    Cir c = {radius, x, y, randuv(-maxV,maxV),randuv(-maxV,maxV),0,0,mass,0};
                    p.addObjects(c);
                }
            }
            endFrame();
        }
    );
    //Button pendulumB(280,425,70,30,"Pendulum", pendulum);
    //Button projectileB(360,425,70,30,"Projectile", projectile);
    //Button orbitalB(40,475,70,30,"Orbital", orbital);
    //Button fluidB(120,475,70,30,"Fluid", fluid);
    //Button harmonicB(200,475,70,30,"Harmonic", harmonic);
    //Button electrostaticB(280,475,70,30,"Electro", electrostatic);
    //Button thermoB(360, 475,70,30,"Thermo", thermo);

    //Add Buttons
    w.addButton(&gravityB);
    w.addButton(&elasticB);
    w.addButton(&inelasticB);
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
                //cout << event.xmotion.x << " PRESS " << event.xmotion.y<< endl;
            }
        }
        p.updatePhysics(DELTA);
        p.collisionDetection();
        p.updateGraphics();
        wait(DELTA);
    }

    getClick();
}
