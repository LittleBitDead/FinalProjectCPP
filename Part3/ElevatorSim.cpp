#include<simplecpp>
#include<queue>
#include<vector>

class Person {
private:
    int currentFloor, nextFloor;
public:
    Person(int cFloor, int nFloor) {
        currentFloor = cFloor;
        nextFloor = nFloor;
    }
    int getRequestedFloor() {
        return nextFloor;
    }
};

class Floor {
private:
    int floorNum;
    queue<Person> upQ;
    queue<Person> downQ;

public:
    Floor(int fNum) {
        floorNum = fNum;
    }
    ~Floor() {
         delete &upQ;
         delete &downQ;
    }
    bool checkUpRequest() {
        return upQ.empty();
    }
    bool checkDownRequest() {
        return downQ.empty();
    }
};

class Elevator {
private:
    int maxCap = 20;
public:
    Elevator(int cap) {
        maxCap = cap;
    }
};

class

int main() {

}
