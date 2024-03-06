#include <simplecpp>
#include <string>
#include "Vector2D.cpp"
#include "Shape.cpp"
#include "Line2D.cpp"
#include "PolyShape.cpp"
#include "PolyGameObject.cpp"
#include "CircleShape.cpp"
#include "CircleGameObject.cpp"
#include "Simulation.cpp"
#include <random>
#include <iostream>

int main(int argc, char* argv[]) {
    int sizeX = 250, sizeY = 250;
    initCanvas("Window", sizeX, sizeY);
    Simulation sim(sizeX, sizeY);

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(-45, 45); // Define the range
    std::uniform_int_distribution<> distrM(15, 25); // Define the range

    int n = 3;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Vector2D origin((i + 1) * 10, (j+1) * 20);
            Vector2D rVel(distr(eng), distr(eng));
            float massR = distrM(eng);
            CircleGameObject *c = new CircleGameObject(massR/2.0f, 0, origin, massR);
            c->setVel(&rVel);
            sim.addGameObject(c);
        }
    }

    cout << "Main Loop Start" << endl;

    while (true) {
        sim.mainLoop();
    }

    getClick();
}
