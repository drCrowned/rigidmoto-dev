#pragma once
#include <box2d/box2d.h>

struct Bike
{
    b2Body *chassis = nullptr;
    b2Body *frontWheel = nullptr;
    b2Body *backWheel = nullptr;

    b2RevoluteJoint *backSuspension = nullptr;
    float wheelRadius = 0.35f;
};

// Create bike at (x, y)
Bike CreateBike(float x, float y);

// Update bike motor
void UpdateBike(Bike &bike, float throttle);
