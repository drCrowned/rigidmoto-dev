#include "bike.hpp"
#include "physics.hpp"

Bike CreateBike(float x, float y)
{
    Bike bike;

    // --- Chassis ---
    b2BodyDef chassisDef;
    chassisDef.type = b2_dynamicBody;
    chassisDef.position.Set(x, y);
    bike.chassis = gWorld->CreateBody(&chassisDef);

    b2PolygonShape chassisShape;
    chassisShape.SetAsBox(1.0f, 0.25f);
    bike.chassis->CreateFixture(&chassisShape, 1.0f);

    // --- Wheels ---
    bike.wheelRadius = 0.35f;
    b2CircleShape wheelShape;
    wheelShape.m_radius = bike.wheelRadius;

    b2BodyDef wheelDef;
    wheelDef.type = b2_dynamicBody;

    // Front wheel
    wheelDef.position.Set(x + 0.8f, y - 0.6f);
    bike.frontWheel = gWorld->CreateBody(&wheelDef);
    bike.frontWheel->CreateFixture(&wheelShape, 1.0f);

    // Back wheel
    wheelDef.position.Set(x - 0.8f, y - 0.6f);
    bike.backWheel = gWorld->CreateBody(&wheelDef);
    bike.backWheel->CreateFixture(&wheelShape, 1.0f);

    // --- Joints ---
    // Front wheel: simple revolute (free rotation)
    {
        b2RevoluteJointDef rjd;
        rjd.Initialize(bike.chassis, bike.frontWheel, bike.frontWheel->GetPosition());
        gWorld->CreateJoint(&rjd);
    }

    // Back wheel: motorized revolute
    {
        b2RevoluteJointDef rjd;
        rjd.Initialize(bike.chassis, bike.backWheel, bike.backWheel->GetPosition());
        rjd.enableMotor = true;
        rjd.motorSpeed = 0.0f;
        rjd.maxMotorTorque = 150.0f; // strong enough to move bike
        bike.backSuspension = (b2RevoluteJoint *)gWorld->CreateJoint(&rjd);
    }

    // --- Fake suspension using distance joints ---
    b2DistanceJointDef dj;
    dj.bodyA = bike.chassis;
    dj.bodyB = bike.frontWheel;
    dj.localAnchorA.Set(0.8f, -0.25f);
    dj.localAnchorB.Set(0, 0);
    dj.length = 0.6f;
    gWorld->CreateJoint(&dj);

    dj.bodyB = bike.backWheel;
    dj.localAnchorA.Set(-0.8f, -0.25f);
    dj.length = 0.6f;
    gWorld->CreateJoint(&dj);

    return bike;
}

void UpdateBike(Bike &bike, float throttle)
{
    if (bike.backSuspension)
        bike.backSuspension->SetMotorSpeed(-throttle * 12.0f);
}
