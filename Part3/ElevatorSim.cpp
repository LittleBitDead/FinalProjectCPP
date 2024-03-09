#include<simplecpp>
#include<queue>
#include<vector>
#include <random>
#include <iostream>
#include <cmath>
enum Status {
    UP, DOWN, IDLE, EMERGENCY
};

struct Person {
    char currentFloor;
    char nextFloor;
};

class Floor {
private:
    char floorNum;
    bool requestUp = false, requestDown = false;
    vector<Person*> waiting;

public:
    Floor(char fNum) {
        floorNum = fNum;
    }

    ~Floor() {
    }

    bool checkUpRequest() {
        return requestUp;
    }

    bool checkDownRequest() {
        return requestUp;
    }

    void addRequest(Person *p) {
        waiting.push_back(p);
        if (p->nextFloor > p->currentFloor) {
            requestUp = true;
        } else {
            requestDown = true;
        }
    }

    Person* service(Status s) {
        return NULL;
    }
};

class Elevator {
private:
    char currentFloor = 0;
    char maxCap = 20;
    vector<Person*> people;
    Status status = Status::IDLE;
public:
    Elevator(char cap) {


        maxCap = cap;
    }

    ~Elevator() {

    }

    char getFloor() {
        return currentFloor;
    }

    Status getStatus() {
        return status;
    }

    void personOn(Person* p) {
        people.push_back(p);
    }

    void personOff(int floor) {
        for (unsigned char i = 0; i < people.size(); i++) {
            if (people[i]->nextFloor == floor) {
                people.erase(people.begin() + i);
            }
        }
    }

    void moveUp() {
        if (status != Status::EMERGENCY) {
            status = Status::UP;
            currentFloor++;
        }
    }

    void moveDown() {
        if (status != Status::EMERGENCY) {
            status = Status::DOWN;
            currentFloor--;
        }
    }

    void emergencyStop() {
        status = Status::EMERGENCY;
    }

    void reset() {
        status = Status::IDLE;
    }


};

class Simulator {
private:
    char floorNum;
    char base = 0;
    vector<Floor> floors;
    Elevator e;
public:
    Simulator(char n, char maxPerFloor, char elevatorCap) : e(elevatorCap) {
        floorNum = n;
        for (char i = 0; i < floorNum; i++) {
            floors.push_back(Floor(i));
        }
    }
    ~Simulator() {
    }

    void processRequests() {
        //No search
        if (e.getStatus() == Status::EMERGENCY) return;
        //Get closest
        if (e.getStatus() == Status::IDLE) {
            int nextFloor = 3;
            for (unsigned char i = 1; i < floorNum; i++) {
                unsigned char temp = (nextFloor + i * pow(-1, i + 1)) % floorNum;
                cout << temp << endl;
            }
        //Up search
        } else if (e.getStatus() == Status::UP) {

        //Down search
        } else {

        }
    }

    void addRequest(char floor, Person* p) {
        if (floor < floorNum && floor >= base) {
            floors[floor].addRequest(p);
        }
    }

};

int main() {
    int width = 520, height = 720;
    initCanvas("Elevator", width, height);
    int numFloor = 10;
    int maxPerFloor = 10;
    int elevatorCap = 15;
    Simulator sim(numFloor, 10, 20);

    int groundOffset = 10;
    int floorOffset = 20;
    int textOffset = 20;
    int floorHeight = (height - groundOffset) / numFloor * 4.0f / 5.0f;
    int left = width / 4, right = width / 2;

    Line b(0,height -  groundOffset, width, height - groundOffset);
    b.imprint();
    Line w(left, height - groundOffset, left, (height - groundOffset) - numFloor * floorHeight);
    w.imprint();
    w.move(right - left, 0);
    w.imprint();


    for (int i = 0; i < numFloor; i++) {
        Text t(left - textOffset, (height - groundOffset) - i * floorHeight - floorHeight/2, i);
        t.imprint();
        Line l(left, (height - groundOffset) - i * floorHeight, right, (height - groundOffset) - i * floorHeight);
        l.imprint();
    }
    Line l(left, (height - groundOffset) - numFloor * floorHeight, right, (height - groundOffset) - numFloor * floorHeight);
    l.imprint();

    int padding = 5;
    int eHeight = floorHeight * 3.0f/4.0f;
    int eWidth = floorHeight / 2;
    Rectangle r(right + 20, height-groundOffset-floorHeight/2, eWidth,eHeight);

    sim.processRequests();

    getClick();
}
