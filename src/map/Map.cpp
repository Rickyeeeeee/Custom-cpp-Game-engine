#include "Map.h"

Map::Map(size_t number, float speed) 
    : Model(number),  
      TriangleNumber(number),
      speed(speed)
{
}

MapModel& Map::getModel() 
{
    return this->Model;    
}

void Map::update() 
{
    Model.update(speed);    
}

void Map::slowSpeed(float value) 
{
    speed -= value;
}

void Map::addSpeed(float value) 
{
    speed += value;
}
