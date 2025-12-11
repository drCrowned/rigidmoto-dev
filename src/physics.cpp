#include "physics.hpp"

b2World *gWorld = nullptr;

void InitPhysics()
{
    b2Vec2 gravity(0.0f, -9.8f);
    gWorld = new b2World(gravity);

    // Create ground
    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, 0.0f); // world origin
    b2Body *ground = gWorld->CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(50.0f, 0.5f); // wide flat ground

    ground->CreateFixture(&groundShape, 0.0f);
}

void StepPhysics(float dt)
{
    gWorld->Step(dt, 8, 3);
}
