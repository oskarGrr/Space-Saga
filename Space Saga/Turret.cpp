#include "Turret.h"
#include "Player.h"
#include "Colision.h"
#include <raylib.h>
#include <cstdlib>

#define PI 3.1415926535f
#define RADIANS_2_DEGREES 180 / PI
#define DEGREES_2_RADIANS PI / 180

Texture2D Turret::gunTurretTexture;
Texture2D Turret::baseTurretTexture;

const float Turret::scale = .2f;
int Turret::turretCount = 4;

Turret::Turret(Vector2 pos)
    : pos(pos), radius(baseTurretTexture.width * scale * 0.5f)
{
}

Turret* Turret::initTurrets(const int screenWidth)
{
    gunTurretTexture = LoadTexture("Resources/Textures/gunSquare.png");
    baseTurretTexture = LoadTexture("Resources/Textures/base.png");

    Turret* turrets = (Turret*)malloc(sizeof(Turret) * turretCount);

    float spaceBetweenTurrets = screenWidth - baseTurretTexture.width * scale * turretCount;
    spaceBetweenTurrets /= (turretCount + 1);

    for(int i = 0; i < turretCount; ++i)
    {
        turrets[i] = Turret({i + gunTurretTexture.width * scale, 5});
        turrets[i].pos.x += spaceBetweenTurrets * (i + 1);
    }
    return turrets;
}

void Turret::drawTurrets(Turret* turrets,Player& player)
{
    float   angleDegrees;
    float   angleRadians;
    Vector2 turret2Player;
    Vector2 offsetRotation;
    Vector2 baseTextureOffset;
    Vector2 GunTextureOffset;
    Vector2 drawPos;

    GunTextureOffset = {-(gunTurretTexture.width * .5f * scale), -(gunTurretTexture.height * .5f * scale)};
    baseTextureOffset = {-(baseTurretTexture.width * .5f * scale), -(baseTurretTexture.height * .5f * scale)};

    for(int i = 0; i < turretCount; ++i)
    {
        turret2Player.x = player.frontNoseCircleCenter.x - turrets[i].pos.x - 45.0f;
        turret2Player.y = player.Y_pos - turrets[i].pos.y; 

        angleRadians = atan2(turret2Player.y, turret2Player.x) - (PI / 2.0f);
        angleDegrees = angleRadians * RADIANS_2_DEGREES;

        offsetRotation = rotateVector(GunTextureOffset, angleRadians);

        drawPos.x = turrets[i].pos.x + offsetRotation.x;
        drawPos.y = turrets[i].pos.y + offsetRotation.y;

        DrawTextureEx(baseTurretTexture, turrets[i].pos, 0, scale, WHITE);
        DrawTextureEx(gunTurretTexture, {drawPos.x + 45.0f, drawPos.y + 40.0f}, angleDegrees, scale, WHITE); 
    }
}
