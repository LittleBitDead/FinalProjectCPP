#pragma once
#include <string>
#include <iostream>
#include <sstream>

class Object {
public:
    virtual std::string toString() const {
        std::stringstream ss;
        ss << this;
        return ss.str();
    }
    virtual bool compareTo(Object *obj) {
        return this == obj;
    }
};
