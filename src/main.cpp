#include "raylib.h"
#include "physics.hpp"
#include "bike.hpp"

const float SCALE = 60.0f; // pixels per meter

void DrawBike(const Bike &bike)
{
    b2Vec2 pos = bike.chassis->GetPosition();
    float angle = bike.chassis->GetAngle();

    float chassisW = 2.0f * SCALE;
    float chassisH = 0.5f * SCALE;

    DrawRectanglePro(
        {pos.x * SCALE, (10 - pos.y) * SCALE, chassisW, chassisH},
        {chassisW * 0.5f, chassisH * 0.5f},
        -angle * RAD2DEG,
        RED);

    b2Body *wheels[2] = {bike.frontWheel, bike.backWheel};
    for (auto wheel : wheels)
    {
        b2Vec2 wpos = wheel->GetPosition();
        DrawCircle(
            wpos.x * SCALE,
            (10 - wpos.y) * SCALE,
            bike.wheelRadius * SCALE,
            BLACK);
    }
}

int main()
{
    InitWindow(1280, 720, "Rigid Bike Prototype");
    InitPhysics();

    // --- Ground ---
    {
        b2BodyDef groundDef;
        groundDef.position.Set(0, 0);
        b2Body *ground = gWorld->CreateBody(&groundDef);

        b2PolygonShape groundShape;
        groundShape.SetAsBox(20.0f, 0.5f);
        ground->CreateFixture(&groundShape, 0.0f);
    }

    Bike bike = CreateBike(5, 5);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        StepPhysics(dt);

        // --- Input ---
        float throttle = 0.0f;
        if (IsKeyDown(KEY_UP))
            throttle = 5.0f;
        if (IsKeyDown(KEY_DOWN))
            bike.backSuspension->SetMotorSpeed(-5.0f);

        float leanTorque = 0.0f;
        if (IsKeyDown(KEY_LEFT))
            leanTorque = 15.0f; // lean left
        if (IsKeyDown(KEY_RIGHT))
            leanTorque = -15.0f; // lean right
        bike.chassis->ApplyTorque(leanTorque, true);

        UpdateBike(bike, throttle);

        // --- Draw ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBike(bike);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
