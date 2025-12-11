#include "raylib.h"
#include "physics.hpp"
#include "bike.hpp"

// Convert Box2D world coords to Raylib pixels
const float SCALE = 30.0f; // tweak to your taste

float ToScreenX(float worldX)
{
    return worldX * SCALE;
}

float ToScreenY(float worldY)
{
    return 720 - (worldY * SCALE); // use your window height
}

void DrawBike(const Bike &bike)
{
    // Draw chassis
    b2Vec2 pos = bike.chassis->GetPosition();
    float angle = bike.chassis->GetAngle();

    float chassisW = 2.0f * SCALE;
    float chassisH = 0.5f * SCALE;

    DrawRectanglePro(
        {ToScreenX(pos.x), ToScreenY(pos.y), chassisW, chassisH},
        {chassisW * 0.5f, chassisH * 0.5f},
        -angle * RAD2DEG, // notice the negative: rotation flips too
        RED);

    // Draw wheels
    b2Body *wheels[2] = {bike.frontWheel, bike.backWheel};
    for (auto wheel : wheels)
    {
        b2Vec2 wpos = wheel->GetPosition();
        DrawCircle(
            ToScreenX(wpos.x),
            ToScreenY(wpos.y),
            bike.wheelRadius * SCALE,
            BLACK);
    }
}

int main()
{
    InitWindow(1280, 720, "Rigid Bike Prototype");
    InitPhysics();

    Bike bike = CreateBike(*gWorld, 10.0f, 10.0f);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        StepPhysics(dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBike(bike);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}