#pragma once

#include "V2.cpp"
#include <graphics.h>

#define FIXED_DELTA 0.003

using namespace std;

struct PhysicsObj2D {
    float mass = 1;
    float charge = 0;
    V2 pos = V2(0,0);
    V2 vel = V2(0,0);
    V2 acc = V2(0,0);
    bool isLocked = false;
    bool infoMode = true;
    virtual void draw() = 0;
    void update(const double D) {
        if (isLocked) return;
        vel += acc * D;
        pos += vel * D;
    };
    void resetAcc() {
        acc.x = 0;
        acc.y = 0;
    }
    void setStatic(bool b) {
        isLocked = b;
    }
};

struct Circle : public PhysicsObj2D {
    float r;
    Circle(double _x, double _y, double _r, float m, float c) {
        pos.x = _x;
        pos.y = _y;
        r = _r;
        mass = m;
        charge = c;
    }
    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        setcolor(colors::BLACK);
        if (isLocked) {
            setfillstyle(SOLID_FILL, colors::BLACK);
            fillellipse(pos.x, pos.y, r, r);
            return;
        }
        if (charge < 0)
            setfillstyle(SOLID_FILL, colors::LIGHTBLUE);
        else if (charge > 0)
            setfillstyle(SOLID_FILL, colors::LIGHTRED);
        else
            setfillstyle(SOLID_FILL, colors::LIGHTGRAY);
        fillellipse(pos.x, pos.y, r, r);

        if (infoMode) {
            setcolor(colors::RED);
            double magV = vel.mag() / 20.0;
            double magA = acc.mag() / 20.0;
            if (magV == 0) magV = 1.0;
            if (magA == 0) magA = 1.0;
            line(pos.x, pos.y, pos.x + vel.x / magV, pos.y + vel.y/magV);
            setcolor(colors::BLUE);
            line(pos.x, pos.y, pos.x + acc.x / magA, pos.y + acc.y / magA);
        }
    }
};

struct Joint2D {
public:
    PhysicsObj2D *from;
    PhysicsObj2D *to;
    virtual void draw() = 0;
    virtual void calcForce() = 0;
};

struct SpringJoint : public Joint2D{
private:
    const int SEGMENTS = 20;
    const int SPRING_WIDTH = 5;
    double length;

public:
    double springConst;
    double damp;

    SpringJoint(PhysicsObj2D *from, PhysicsObj2D *to, double c, double l, double d) {
        springConst = c;
        length = l;
        damp = d;
        this->from = from;
        this->to = to;
    }

    void draw() {
        setlinestyle(line_styles::SOLID_LINE, 0, 1);
        setcolor(colors::BLACK);
        if (damp > 0) setcolor(colors::GREEN);
        V2 stepDis = (to->pos - from->pos) * (1.0/SEGMENTS);
        V2 normal(-stepDis.y, stepDis.x);
        normal *= (1.0/normal.mag());

        V2 cur = from->pos;
        V2 last = from->pos;
        short swap = 1;
        for (int i = 0; i < SEGMENTS - 1; i++) {
            cur = from->pos + stepDis * (i + 1) + normal * SPRING_WIDTH * swap;
            line(last.x, last.y, cur.x, cur.y);
            last = cur;
            swap *= -1;
        }
        line(last.x, last.y, to->pos.x, to->pos.y);
    }

    void calcForce() {
        V2 deltaC = to->pos - from->pos;
        double deltaD = deltaC.mag() - length;
        double force = -springConst * deltaD;
        V2 dampForceF = from->vel * damp;
        V2 dampForceT = to->vel * -damp;

        double angle = atan2(deltaC.y, deltaC.x);
        V2 forceV = {cos(angle) * force, sin(angle) * force};
        from->acc -= forceV * (1.0/from->mass) + dampForceF;
        to->acc += forceV * (1.0/to->mass) + dampForceT;
    }
};

struct RigidJoint: public Joint2D {
    double length;
    const double TOLERANCE = 0.01;
    RigidJoint(PhysicsObj2D *from, PhysicsObj2D *to, double l) {
        length = l;
        this->from = from;
        this->to = to;
    }

    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        setcolor(colors::BLACK);
        line(from->pos.x, from->pos.y, to->pos.x, to->pos.y);
    }

    void calcForce() override {
        V2 relPos = to->pos - from->pos;
        double dist = relPos.mag();
        double offset = length - dist;
        if (abs(offset) > 0.0) {
            V2 norm = relPos * (1.0/dist);
            V2 relVel = to->vel - from->vel;

            double consMass = (1.0/to->mass) + (1.0/from->mass);

            double velDot = norm.dot(relVel);
            double biasF = 0.1;
            double bias = -(biasF/FIXED_DELTA) * offset * 10;
            double lambda = -(velDot + bias) / consMass;
            V2 toImp = norm * lambda;
            V2 fromImp = norm * -lambda;

            to->vel += toImp * (1.0/to->mass);
            from->vel += fromImp * (1.0/from->mass);
        }
    }
};

struct HingeJoint: public Joint2D {
    double length;
    const double TOLERANCE = 0.01;
    HingeJoint(PhysicsObj2D *from, PhysicsObj2D *to, double l) {
        length = l;
        this->from = from;
        from->isLocked = true;
        this->to = to;
    }

    void draw() override {
        setlinestyle(line_styles::SOLID_LINE, 0, 2);
        setcolor(colors::BLACK);
        line(from->pos.x, from->pos.y, to->pos.x, to->pos.y);
    }

    void calcForce() override {
        V2 relPos = to->pos - from->pos;
        V2 norm = relPos * (1.0/relPos.mag());
        V2 perp(-norm.y, norm.x);

        double dist = relPos.mag();
        double offset = length - dist;
        double angle = atan2(relPos.y, relPos.x);
        V2 perpAcc = (perp * (to->acc.mag() * cos(angle)));
        to->acc = perpAcc;
        to->vel -= (norm * (norm.dot(to->vel)));
        to->pos = (from->pos + (norm * length));
        }
};
