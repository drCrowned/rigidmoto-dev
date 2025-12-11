#pragma once
#include <box2d/box2d.h>

struct Bike
{
    b2Body *chassis = nullptr;
    b2Body *frontWheel = nullptr;
    b2Body *backWheel = nullptr;

    float wheelRadius = 0.35f;
};

Bike CreateBike(b2World &world, float x, float y);
