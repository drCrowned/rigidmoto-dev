// physics.cpp

#include "physics.hpp"

b2World *gWorld = nullptr;

void InitPhysics()
{
    b2Vec2 gravity(0.0f, -9.8f);
    gWorld = new b2World(gravity);

    // Create ground (This is the ONLY place the ground should be created)
    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, 0.0f); // Ground starts at the world origin
    b2Body *ground = gWorld->CreateBody(&groundDef);

    b2PolygonShape groundShape;
    // Make it wide enough to cover the screen area (20 units in your drawing scale)
    groundShape.SetAsBox(50.0f, 0.5f);

    // Static bodies have a mass of 0, so the fixture density is ignored.
    ground->CreateFixture(&groundShape, 0.0f);
}

void StepPhysics(float dt)
{
    // Use the gWorld pointer to step the simulation
    if (gWorld)
    {
        gWorld->Step(dt, 8, 3);
    }
}