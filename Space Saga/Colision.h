#pragma once
#include <raylib.h>
#include "Invader.h"
#include "declarationsAndGameloops.h"


bool isColliding(Vector2 p1, Vector2 p2, float r1, float r2);
Vector2 normalizeVector2(Vector2 v);
Vector2 rotateVector(Vector2 pos, float radians);
Vector2 rotateAroundOrigin(Vector2, Vector2, float); 
void drawInvadersCircleTest(Invader* invaders, const int index);