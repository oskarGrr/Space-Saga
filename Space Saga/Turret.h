#pragma once
#include <raylib.h>

class Player;
class Bullet;

class Turret
{
public:
    Vector2 pos;
    float   radius;
    Vector2 baseTextureOffset;
    Vector2 gunTextureOffset; 
    float   amountRotated;
    Vector2 rotationOffset;
    Vector2 hitCircle;

    static Texture2D   baseTurretTexture;
    static Texture2D   gunTurretTexture;
    static const float scale;
    static const int   turretCount;
    static char        validTurrets;

    Turret(Vector2 pos);
    static void initTurrets(const int SW, const int SH, Turret* turrets);
    static void drawTurrets(Turret* turrets, Player & player, Bullet* bullets);
};