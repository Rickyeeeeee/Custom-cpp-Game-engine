#pragma once

#include "Triangle.h"
#include "MapModel.h"

#include <vector>

class Map
{
public:
    Map(size_t number, float speed);
    MapModel& getModel();
    void update();
    void slowSpeed(float value);
    void addSpeed(float value);
private:
    float speed;
    size_t TriangleNumber;
    MapModel Model;
};