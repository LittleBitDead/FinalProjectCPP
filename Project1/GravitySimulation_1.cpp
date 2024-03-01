#include <simplecpp>
#include <string>
#include "Vector2D.cpp"
#include "Shape.cpp"
#include "PolyShape.cpp"

int main(int argc, char* argv[]) {
    initCanvas("Window", 250, 250);
    //Line l(0,0,100,100);
    Vector2D vec[] = {Vector2D(10,10), Vector2D(50,10), Vector2D(50,50), Vector2D(10,50)};
    PolyShape s(*vec);
    cout << s.toString() << endl;
    s.draw();

    getClick();
}
