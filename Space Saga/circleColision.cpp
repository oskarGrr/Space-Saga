#include "Colision.h"
#include <raylib.h>
#include "declarationsAndGameloops.h"
#include <cmath>

bool isColliding(Vector2 p0, Vector2 p1, float r0, float r1)
{
    Vector2 length;
    float lengthSqrd;
    float radiusSum;
    float radiusSquared;

    length.x = p1.x - p0.x;
    length.y = p1.y - p0.y;
    lengthSqrd = (length.x * length.x) + (length.y * length.y);

    radiusSum = r0 + r1;
    radiusSquared = radiusSum * radiusSum;

    if (lengthSqrd < radiusSquared) return true;
    return false;
}

Vector2 normalizeVector2(Vector2 v)
{
    float magnitude;
    magnitude = sqrt((v.x * v.x) + (v.y * v.y));
    return {v.x / magnitude, v.y / magnitude};
}

Vector2 rotateVector(Vector2 vec, float radians)
{
    Vector2 rotatedVec;
    rotatedVec.x = (vec.x * cos(radians)) - (vec.y * sin(radians));
    rotatedVec.y = (vec.x * sin(radians)) + (vec.y * cos(radians));
    return rotatedVec;
}

Vector2 rotateAroundOrigin(Vector2 origin, Vector2 pos, float radians)
{
    Vector2 posRelative;
    posRelative.x = pos.x - origin.x;
    posRelative.y = pos.y - origin.y;

    Vector2 rotatedPos;
    rotatedPos.x = (posRelative.x * cos(radians)) - (posRelative.y * sin(radians));
    rotatedPos.y = (posRelative.x + sin(radians)) + (posRelative.y * cos(radians));

    return {origin.x + rotatedPos.x, origin.y + rotatedPos.y};
}

void drawInvadersCircleTest(Invader* invaders, const int index)//for invaders
{
    DrawCircleLines(invaders[index].center_wingLargeLeft.x, invaders[index].center_wingLargeLeft.y, invaders[index].wingRadiusLarge, SKYBLUE);
    DrawCircleLines(invaders[index].center_wingLargeRight.x, invaders[index].center_wingLargeRight.y, invaders[index].wingRadiusLarge, SKYBLUE);
    DrawCircleLines(invaders[index].noseTipCircleCenter.x, invaders[index].noseTipCircleCenter.y, invaders[index].noseTipRadius, SKYBLUE);
}