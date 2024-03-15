#include <simplecpp>
#include "Window.cpp";
int main() {
    initCanvas();
    Window w();

    XEvent event;
    while (true) {
        nextEvent(event);
        if (mouseButtonPressEvent(event)) {
        }
    }
    getClick();
}
