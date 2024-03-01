#pragma once
#include <simplecpp>
#include "Physics2D.cpp"
#include "Vector2D.cpp"
#include "Draw.cpp"

#define loopPoints(pointsArrayPointer) int i = 0; for (Vector2D *p = pointsArrayPointer; p <= pointsArrayPointer + sizeof(Vector2D)/sizeof(pointsArrayPointer); ++p, i++)

class Shape : public Physics2D, public Draw {
};

