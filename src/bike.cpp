#include "bike.hpp"
#include "physics.hpp"

Bike CreateBike(b2World &world, float x, float y)
{
    Bike bike;

    // create chassis
    b2BodyDef chassisDef;
    chassisDef.type = b2_dynamicBody;
    chassisDef.position.Set(x, y);
    bike.chassis = world.CreateBody(&chassisDef);

    b2PolygonShape chassisShape;
    chassisShape.SetAsBox(1.0f, 0.25f);
    bike.chassis->CreateFixture(&chassisShape, 1.0f);

    // wheels
    bike.wheelRadius = 0.35f;

    b2BodyDef wheelDef;
    wheelDef.type = b2_dynamicBody;

    // front wheel
    wheelDef.position.Set(x + 0.8f, y - 0.4f);
    bike.frontWheel = world.CreateBody(&wheelDef);

    b2CircleShape wheelShape;
    wheelShape.m_radius = bike.wheelRadius;
    bike.frontWheel->CreateFixture(&wheelShape, 1.0f);

    // back wheel
    wheelDef.position.Set(x + 0.8f, y - 0.4f);
    bike.backWheel = world.CreateBody(&wheelDef);
    bike.backWheel->CreateFixture(&wheelShape, 1.0f);

    return bike;
}