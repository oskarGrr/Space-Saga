#include "Turret.h"
#include "Player.h"
#include "utils.h"
#include <raylib.h>
#include <cstdlib>
#include <iostream>

Texture2D Turret::gunTurretTexture;
Texture2D Turret::baseTurretTexture;

const float Turret::scale = .2f;
const int Turret::turretCount = 4;
char Turret::validTurrets = 0b00001111;

Turret::Turret(Vector2 pos)
    : pos(pos), radius(34), 
      baseTextureOffset( {baseTurretTexture.width / 2 * scale, baseTurretTexture.height / 2 * scale} ),
      gunTextureOffset( {gunTurretTexture.width / 2 * scale, gunTurretTexture.height / 2 * scale} ), 
      amountRotated(0), rotationOffset({-(gunTurretTexture.width / 2 * scale), -(gunTurretTexture.height / 2 * scale)}),
      hitCircle(pos)
{
}

void Turret::initTurrets(const int SW, const int SH, Turret* turrets)
{
    gunTurretTexture = LoadTexture("Resources/Textures/gun.png");
    baseTurretTexture = LoadTexture("Resources/Textures/base.png");

    float spaceBetweenTurrets = SW / (turretCount + 1);
    
    for(int i = 1; i < turretCount + 1; ++i)
        turrets[i-1] = Turret({spaceBetweenTurrets * i, SH * .065f});
}

void Turret::drawTurrets(Turret* turrets,Player& player, Bullet* bullets)
{
    Vector2 turret2Player; 
    
    for(int i = 0; i < turretCount; ++i)
    {
        if(!(validTurrets & 1 << i)) continue;

        turret2Player.x = player.pos.x - turrets[i].pos.x;
        turret2Player.y = player.pos.y - turrets[i].pos.y;
 
        turrets[i].amountRotated = atan2(turret2Player.y, turret2Player.x) - (PI / 2.0f);
                                        
        turrets[i].rotationOffset = {-(gunTurretTexture.width / 2 * scale), -(gunTurretTexture.height / 2 * scale)};
        turrets[i].rotationOffset = Utils::rotateVector(turrets[i].rotationOffset, turrets[i].amountRotated);
        
        turrets[i].rotationOffset.x += turrets[i].pos.x;
        turrets[i].rotationOffset.y += turrets[i].pos.y;

        DrawTextureEx(baseTurretTexture, {turrets[i].pos.x -turrets[i].baseTextureOffset.x, 
                                          turrets[i].pos.y -turrets[i].baseTextureOffset.y}, 0, scale, WHITE);
        DrawTextureEx(gunTurretTexture, turrets[i].rotationOffset, turrets[i].amountRotated * RAD2DEG, scale, WHITE);
    }
}