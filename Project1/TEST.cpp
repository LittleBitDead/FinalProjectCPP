#include <iostream>
#include <cmath>

struct Circle {
    float x, y;  // Center coordinates
    float vx, vy; // Velocities
};

// Function to calculate the distance between two points
float distance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Function to calculate the relative velocity between two circles
void relativeVelocity(const Circle& c1, const Circle& c2, float& rvx, float& rvy) {
    rvx = c1.vx - c2.vx;
    rvy = c1.vy - c2.vy;
}

// Function to calculate the final velocities after collision
void collisionSpeeds(Circle& c1, Circle& c2) {
    float m1 = 1.0; // Mass of circle 1 (assuming equal masses for simplicity)
    float m2 = 1.0; // Mass of circle 2

    float rvx, rvy;
    relativeVelocity(c1, c2, rvx, rvy);

    float normalX = c2.x - c1.x;
    float normalY = c2.y - c1.y;
    float distance = std::sqrt(normalX * normalX + normalY * normalY);
    normalX /= distance;
    normalY /= distance;

    // Calculate the relative velocity along the line of impact
    float rv = rvx * normalX + rvy * normalY;

    // Calculate the final velocities using the conservation of momentum and kinetic energy
    float v1f = (m1 * c1.vx + m2 * c2.vx - m2 * (c1.vx - c2.vx)) / (m1 + m2);
    float v2f = (m1 * c1.vx + m2 * c2.vx - m1 * (c2.vx - c1.vx)) / (m1 + m2);

    c1.vx = v1f;
    c2.vx = v2f;
}

int main() {
    // Define two circles with initial positions and velocities
    Circle circle1 = {0, 0, 2, 0}; // Circle 1: Center at (0, 0), velocity (2, 0)
    Circle circle2 = {4, 0, -1, 0}; // Circle 2: Center at (4, 0), velocity (-1, 0)

    // Calculate speeds after collision
    collisionSpeeds(circle1, circle2);

    // Print the speeds after collision
    std::cout << "Circle 1 speed after collision: " << std::sqrt(circle1.vx * circle1.vx + circle1.vy * circle1.vy) << std::endl;
    std::cout << "Circle 2 speed after collision: " << std::sqrt(circle2.vx * circle2.vx + circle2.vy * circle2.vy) << std::endl;

    return 0;
}
