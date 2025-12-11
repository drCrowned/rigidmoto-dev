#pragma once
#include <box2d/box2d.h>

extern b2World *gWorld;

void InitPhysics();
void StepPhysics(float dt);
