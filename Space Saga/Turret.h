#pragma once
#include <raylib.h>

class Player;

class Turret
{
public:
    Vector2 pos;
    float radius;

    static Texture2D baseTurretTexture;
    static Texture2D gunTurretTexture;
    static const float scale;
    static int turretCount;

    Turret(Vector2 pos);
    static Turret* initTurrets(const int screenWidth);
    static void drawTurrets(Turret* turrets, Player & player);
};