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
const float delta = 0.02;

void gravityFunc(Cir &c1, Cir &c2) {
    const float G = 0.1;
    float dx = c2.x - c1.x;
    float dy = c2.y - c1.y;
    float force = (G * c1.mass * c2.mass) / (dx * dx + dy * dy);
    c1.vx += force * dx / c1.mass;
    c1.vy += force * dy / c1.mass;
    c2.vx -= force * dx / c2.mass;
    c2.vy -= force * dy / c2.mass;
}
void gravity(PhysicsSim *p) {
    cout << "Clearing Previous Sim!" << endl;
    p->clearSim();
    cout << "Starting Gravity Sim!" << endl;
    Cir c1 = {25, 0, 0, -5, -5, 1000, 1};
    Cir c2 = {3, 50, 30, 0, 0, 10, -1};
    Cir c3 = {6, -30, -70, 10, 20, 20, 0};
    p->setForceFunction(gravityFunc);
    p->addObjects(c1);
    p->addObjects(c2);
    p->addObjects(c3);
}
void elastic(PhysicsSim *p) {
    cout << "Clearing Previous Sim!" << endl;
    p->clearSim();
    cout << "Starting Elastic Sim!" << endl;

    int numCol = 10, numRow = 5;
    float offsetX = p->getW()/numCol;
    float offsetY = p->getH()/numRow;
    float radius = offsetX / 5;
    float mass = 5;

    float maxV = 10;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-maxV, maxV);

    beginFrame();
    for (int i = numCol/2 - numCol; i <= numCol/2; i++) {
        for (int j = numRow/2 - numRow; j <= numRow/2; j++) {
            Cir c = {radius,(float) i * offsetX,(float) j * offsetY, dis(gen), dis(gen), mass, 0};
            p->addObjects(c);
        }
    }
    endFrame();
}
void inelastic(PhysicsSim *p) {
    cout << "Clearing Previous Sim!" << endl;
    p->clearSim();
    cout << "Starting Inelastic Sim!" << endl;

    int numCol = 10, numRow = 5;
    float offsetX = p->getW()/numCol;
    float offsetY = p->getH()/numRow;
    float radius = offsetX / 5;
    float mass = 5;

    float maxV = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-maxV, maxV);

    beginFrame();
    for (int i = numCol/2 - numCol; i <= numCol/2; i++) {
        for (int j = numRow/2 - numRow; j <= numRow/2; j++) {
            Cir c = {radius,(float) i * offsetX,(float) j * offsetY, dis(gen), dis(gen), mass, 0};
            p->addObjects(c);
        }
    }
    endFrame();
    p->setElasticity(0.8);
}
void pendulum(PhysicsSim *p) {
    cout << "Starting Pendulum Sim!" << endl;
}
void projectile(PhysicsSim *p) {
    cout << "Starting Projectile Sim!" << endl;
}
void orbital(PhysicsSim *p) {
    cout << "Starting Orbital Sim!" << endl;
}
void fluid(PhysicsSim *p) {
    cout << "Starting Fluid Sim!" << endl;
}
void harmonic(PhysicsSim *p) {
    cout << "Starting Harmonic Sim!" << endl;
}
void electrostatic(PhysicsSim *p) {
    cout << "Starting Electrostatic Sim!" << endl;
}
void thermo(PhysicsSim *p) {
    cout << "Starting Thermo Sim!" << endl;
}

int main() {

    initCanvas("Simulator", WIDTH, HEIGHT);
    Window w(WIDTH/2, HEIGHT - HEIGHT/sizeW, WIDTH, HEIGHT/sizeW * 2, "Options");
    PhysicsSim p(WIDTH/2, HEIGHT/sizeW * 2, WIDTH, HEIGHT/sizeW * 4, "Physics Sim");
    //InputBox in(WIDTH/2, HEIGHT/2, 100, 100, "Hello");

    //getClick();
    //cout << in.getInput("Hello") << "<- INPUT" << endl;
    //getClick();

    beginFrame();
    //Create Buttons
    Button gravityB(40,425,70,30,"Gravity", gravity);
    Button elasticB(120,425,70,30,"Elastic", elastic);
    Button inelasticB(200,425,70,30,"Inelastic", inelastic);
    Button pendulumB(280,425,70,30,"Pendulum", pendulum);
    Button projectileB(360,425,70,30,"Projectile", projectile);
    Button orbitalB(40,475,70,30,"Orbital", orbital);
    Button fluidB(120,475,70,30,"Fluid", fluid);
    Button harmonicB(200,475,70,30,"Harmonic", harmonic);
    Button electrostaticB(280,475,70,30,"Electro", electrostatic);
    Button thermoB(360, 475,70,30,"Thermo", thermo);

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
    endFrame();

    //Mouse
    XEvent event;

    event.xmotion.x = 0;
    event.xmotion.y = 0;

    nextEvent(event);
    while (true) {
        if (checkEvent(event)) {
            if (mouseButtonReleaseEvent(event)) {
                w.handleClick(event.xmotion.x, event.xmotion.y, &p);
                p.handleIteract(event.xmotion.x, event.xmotion.y);
            }
        }

        p.updatePhysics(delta);
        p.collisionDetection();
        p.updateGraphics();
        wait(delta);
    }

    getClick();
}
