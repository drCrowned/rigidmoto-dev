#pragma once
#include <box2d/box2d.h>

struct Bike
{
    b2Body *chassis = nullptr;
    b2Body *frontWheel = nullptr;
    b2Body *backWheel = nullptr;

    // Change from b2RevoluteJoint to b2WheelJoint for realistic suspension
    b2WheelJoint *frontSuspension = nullptr;
    b2WheelJoint *backSuspension = nullptr; // This will also handle the motor

    float wheelRadius = 0.35f;
};

// Create bike at (x, y)
Bike CreateBike(float x, float y);

// Update bike motor
void UpdateBike(Bike &bike, float throttle);