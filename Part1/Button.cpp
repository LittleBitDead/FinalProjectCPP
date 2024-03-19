#pragma once
#include <simplecpp>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <string>

class Button {
private:
    int posX, posY;
    int width, height;
    Rectangle r;
    Text label;
    function<void()> onClick;
public:
    Button(int x, int y, int w, int h, string str, function<void()> onClickF) :
            posX(x), posY(y), width(w), height(h), onClick(onClickF) {
        r = Rectangle(posX, posY, width, height);
        label = Text(posX, posY, str);
        r.imprint();
        label.imprint();
    }
    bool containsPoint(int x, int y) {
        if (x < posX - width/2 || x > posX + width/2) return false;
        if (y < posY - height/2 || y > posY + height/2) return false;
        return true;
    }
    void click() {
        if (onClick != NULL) onClick();
    }
};
