#include "bike.hpp"
#include "physics.hpp"

// Suspension constants
const float SUSPENSION_FREQUENCY_HZ = 2.0f;  // Spring frequency (Hz)
const float SUSPENSION_DAMPING_RATIO = 0.2f; // Damping ratio (0.7 is common for vehicles)
const float MAX_MOTOR_TORQUE = 160.0f;       // Max torque for the rear motor

// Calculates the stiffness (k) for a Wheel Joint based on frequency and damping ratio.
// Box2D uses this formula in its testbed examples.
float b2GetWheelJointStiffness(float frequencyHz, float dampingRatio, b2Body *bodyA, b2Body *bodyB)
{
    if (frequencyHz <= 0.0f)
    {
        return 0.0f;
    }
    float mass = bodyA->GetMass() + bodyB->GetMass();
    float omega = 2.0f * b2_pi * frequencyHz;
    return mass * omega * omega;
}

// Calculates the damping coefficient (c) for a Wheel Joint.
float b2GetWheelJointDamping(float frequencyHz, float dampingRatio, b2Body *bodyA, b2Body *bodyB)
{
    if (frequencyHz <= 0.0f)
    {
        return 0.0f;
    }
    float mass = bodyA->GetMass() + bodyB->GetMass();
    float omega = 2.0f * b2_pi * frequencyHz;
    return 2.0f * mass * dampingRatio * omega;
}

Bike CreateBike(float x, float y)
{
    Bike bike;

    // --- Chassis ---
    b2BodyDef chassisDef;
    chassisDef.type = b2_dynamicBody;
    chassisDef.position.Set(x, y);

    // 1. Set ALL properties of the definition *BEFORE* creating the body
    chassisDef.angularDamping = 6.0f; // Resists motor torque
    chassisDef.linearDamping = 1.0f;  // Resists linear motion

    // 2. Create the body *ONCE*
    bike.chassis = gWorld->CreateBody(&chassisDef);

    b2PolygonShape chassisShape;
    chassisShape.SetAsBox(1.0f, 0.25f);

    // 3. Create the fixture *ONCE*
    // NOTE: The fixture takes the density, but we already set the density
    // on the body definition above, so we can pass any value here for density,
    // but typically we pass the mass density here. Let's stick with 0.5f.
    bike.chassis->CreateFixture(&chassisShape, 0.5f);

    // --- Wheels ---
    bike.wheelRadius = 0.35f;
    b2CircleShape wheelShape;
    wheelShape.m_radius = bike.wheelRadius;

    b2BodyDef wheelDef;
    wheelDef.type = b2_dynamicBody;

    // Wheel mass is often higher than the chassis fixture mass for inertia
    const float wheelDensity = 2.5f;

    // Front wheel
    float frontX = x + 0.8f;
    float frontY = y - 0.6f;
    wheelDef.position.Set(frontX, frontY);
    bike.frontWheel = gWorld->CreateBody(&wheelDef);
    bike.frontWheel->CreateFixture(&wheelShape, wheelDensity);

    // Back wheel
    float backX = x - 0.8f;
    float backY = y - 0.6f;
    wheelDef.position.Set(backX, backY);
    bike.backWheel = gWorld->CreateBody(&wheelDef);
    bike.backWheel->CreateFixture(&wheelShape, wheelDensity);

    // --- Suspension (b2WheelJoint) ---
    b2WheelJointDef wjd;
    wjd.bodyA = bike.chassis;

    // Suspension Axis
    wjd.localAxisA.Set(0.0f, -1.0f);
    wjd.collideConnected = false;

    // --- Front Suspension ---
    wjd.bodyB = bike.frontWheel; // <-- CRITICAL: Set bodyB FIRST!

    wjd.enableLimit = true;
    wjd.lowerTranslation = -0.2f; // Bottomed out (Wheel is 0.2m closer to chassis)
    wjd.upperTranslation = 0.1f;  // Topped out (Wheel is 0.1m farther from chassis)

    // Anchor Points
    wjd.localAnchorA.Set(0.8f, -0.25f);
    wjd.localAnchorB.Set(0.0f, 0.0f);

    // NOW calculate stiffness/damping after both bodyA and bodyB are set
    wjd.stiffness = b2GetWheelJointStiffness(SUSPENSION_FREQUENCY_HZ, SUSPENSION_DAMPING_RATIO, wjd.bodyA, wjd.bodyB);
    wjd.damping = b2GetWheelJointDamping(SUSPENSION_FREQUENCY_HZ, SUSPENSION_DAMPING_RATIO, wjd.bodyA, wjd.bodyB);

    // Create Joint
    bike.frontSuspension = (b2WheelJoint *)gWorld->CreateJoint(&wjd);

    // --- Back Suspension (Motorized) ---
    wjd.bodyB = bike.backWheel; // <-- CRITICAL: Set bodyB FIRST!

    // Anchor Points (only localAnchorA needs to change)
    wjd.localAnchorA.Set(-0.8f, -0.25f);

    // Motor settings
    wjd.enableMotor = true;
    wjd.motorSpeed = 0.0f;
    wjd.maxMotorTorque = MAX_MOTOR_TORQUE;

    // Recalculate stiffness/damping
    wjd.stiffness = b2GetWheelJointStiffness(SUSPENSION_FREQUENCY_HZ, SUSPENSION_DAMPING_RATIO, wjd.bodyA, wjd.bodyB);
    wjd.damping = b2GetWheelJointDamping(SUSPENSION_FREQUENCY_HZ, SUSPENSION_DAMPING_RATIO, wjd.bodyA, wjd.bodyB);

    // Create Joint
    bike.backSuspension = (b2WheelJoint *)gWorld->CreateJoint(&wjd);

    return bike;
}

void UpdateBike(Bike &bike, float throttle)
{
    // Motor is now set on the b2WheelJoint, which works the same way.
    if (bike.backSuspension)
        bike.backSuspension->SetMotorSpeed(-throttle * 12.0f);
}