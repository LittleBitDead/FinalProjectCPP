#include <simplecpp>
#include <functional>
#include <vector>

class Element {
protected:
    int x, y;
    int w, h;
    bool enabled;
public:
    Element(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {};
    void enable() { enabled = true; };
    void disable() { enabled = false; };
    int getWidth() { return w; }
    int getHeight() { return h; }
};

class TextBox : public Element {
protected:
    Text t;
    Rectangle r;
    Color c;
    int p;
public:
    TextBox(int _x, int _y, string _s = "TEXT", int _p = 10, Color _c = COLOR(255,255,255)) :
    Element(_x,_y,textWidth(_s) + _p, textHeight() + _p), p(_p), c(_c)
    {
        r = Rectangle(x,y,w + p,h + p);
        r.setColor(c);
        t = Text(x,y,_s);

    }
    void enable() {
        Element::enable();
        t.show();
        r.show();
    }

    void disable()  {
        Element::disable();
        t.hide();
        r.hide();
    }
};

class ButtonBox : public TextBox {
protected:
     function<void()> func;
public:
    ButtonBox(int _x, int _y, string _s = "BUTTON", int _p = 10, Color _c = COLOR(225,175,175), function<void()> onClick = NULL) :
    TextBox(_x, _y, _s, _p, _c), func(onClick) {}

    void click() {
        if (func != NULL) func();
    }

    bool checkClick(int xPos, int yPos) {
        if (enabled == false) return false;
        if (xPos < x - w/2 || xPos > x + w/2) return false;
        if (yPos < y - h/2 || yPos > y + h/2) return false;
        return true;
    }
};

class Window : public Element {
protected:
    int rows, columns;

public:
    Window() : rows(0), columns(0){

    }

    void addElement(Element e, int r, int c) {

    }
};

class Simulation: public Window {

};


