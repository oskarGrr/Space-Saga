#include "Bullet.h"
#include "Turret.h"
#include "player.h"
#include "utils.h"
#include "invader.h"
#include <chrono>
#include <cmath>

#include <iostream>

int   Bullet::count;
int   Bullet::max_bullets = 16;
float Bullet::speedMultiplier = 700.0f;
float Bullet::Tscale = 0.6f;
float Bullet::Iscale = 1.4f;
Texture2D Bullet::greenLaser;
Texture2D Bullet::redLaser;
Texture2D Bullet::turretLaser;

void Bullet::init_Bullet_Textures()
{
    greenLaser = LoadTexture("Resources/Textures/greenLaser.png");
    redLaser = LoadTexture("Resources/Textures/redLaser.png");
    turretLaser = LoadTexture("Resources/Textures/turretLaser.png");
}

Bullet::Bullet(Vector2 InitialPos, TEAM team)
    : pos(InitialPos),
      team(team), currentlyDrawnOrNot(currentlyDrawnOrNot::OFF_SCREEN)
{
    if(team == TEAM::INVADER)
    {
        invaderRadius = 6.0f;
        textureOffset = {redLaser.width * Iscale / 2, redLaser.height * Iscale / 2};
    }
    else if (team == TEAM::PLAYER)
    {
        playerRadius = 10.0f;
        textureOffset = {greenLaser.width * Tscale / 2, greenLaser.height * Tscale / 2};
    }
    else if(team == TEAM::TURRET)
    { 
        turretRadius = 8.0f;
    }
    rotatedOffset = {};    
}

void Bullet::manageBullets(Bullet* bullets, Invader* invaders, Player& player, Turret* turrets, float deltaTime)
{
    updatePlayerBullets(bullets, player.pos);
    updateInvaderBullets(invaders, bullets);
    updateTurretBullets(bullets, turrets, player, deltaTime);
}

void Bullet::updatePlayerBullets(Bullet* bullets, Vector2 playerPos)
{
    using namespace std::chrono;

    steady_clock::time_point end = steady_clock::now();
    static duration<float> timeElapsed = steady_clock::duration::zero();   
    static steady_clock::time_point start = steady_clock::now();
    
    timeElapsed = end - start;

    if(timeElapsed.count() > 0.65f)
    { 
        if(IsMouseButtonPressed(MouseButton::MOUSE_LEFT_BUTTON))
        {
            start = steady_clock::now();

            bullets[count] = Bullet(playerPos, TEAM::PLAYER);
            bullets[count].currentlyDrawnOrNot = currentlyDrawnOrNot::ON_SCREEN;

            count = (count + 1) % max_bullets;
        }
        return;
    }
}

void Bullet::updateInvaderBullets(Invader* invaders, Bullet* bullets)
{
    if(!Invader::validInvaders) return; // return if no invaders alive
    using namespace std::chrono;

    static steady_clock::time_point start = steady_clock::now();
    steady_clock::time_point        end = steady_clock::now();
    static duration<float>          timeElapsed = steady_clock::duration::zero();

    timeElapsed = end - start;
    if(timeElapsed.count() > Invader::shootSpeed)
    {
        start = steady_clock::now();
        int rng = 0;

        for( ; ; )
        {//loops until rand() picks an invader that is alive          
            rng = rand() % Invader::max_invaders;
            if(Invader::validInvaders & 1 << rng) break;
        }
        bullets[count] = Bullet(invaders[rng].pos, TEAM::INVADER);
        bullets[count].currentlyDrawnOrNot = currentlyDrawnOrNot::ON_SCREEN;    
        count = (count + 1) % max_bullets;

        return;
    }   
}

void Bullet::updateTurretBullets(Bullet* bullets, Turret* turrets, Player& player, float deltaTime)
{
    if(!Turret::validTurrets) return; // return if no turrets alive
    using namespace std::chrono;

    static steady_clock::time_point start = steady_clock::now();
    static duration<float> timeElapsed = steady_clock::duration::zero();
    steady_clock::time_point end = steady_clock::now();
    
    timeElapsed = end - start;
    
    if(timeElapsed.count() > 3.8f)
    {
        Bullet& b = bullets[count];
        int rngTurret = 0;

        //pick rng turret to shoot from
        for( ; ; )
        {//breaks when it picks on that is alive
            rngTurret = rand() % Turret::turretCount;
            if(Turret::validTurrets & 1 << rngTurret) break;
        }

        const float spawnOffset = 75.0f;

        Vector2 spawnPos = {turrets[rngTurret].pos.x, turrets[rngTurret].pos.y + spawnOffset};
        spawnPos = Utils::rotateAroundSpecificPoint(spawnPos, turrets[rngTurret].pos, turrets[rngTurret].amountRotated);

        //initialize new bullet at rng turret location
        b = Bullet(spawnPos, TEAM::TURRET);
        
        b.currentlyDrawnOrNot = currentlyDrawnOrNot::ON_SCREEN;

        {//after they are in the right place we can rotate the bullet to point to the player
            Vector2 laserTextureOffset = {-(turretLaser.width * .5f * Tscale),-(turretLaser.height * .5f * Tscale)};
            b.rotatedOffset = Utils::rotateVector(laserTextureOffset, turrets[rngTurret].amountRotated);  

            b.rotatedOffset.x += b.pos.x;
            b.rotatedOffset.y += b.pos.y;

            b.angleDegrees = turrets[rngTurret].amountRotated * RAD2DEG;
        }

        {//gets vector to player and sets the right length
            b.velocityVector = {player.pos.x - b.pos.x, 
                                player.pos.y - b.pos.y};
            b.velocityVector = Utils::normalizeVector2(b.velocityVector);
            b.velocityVector.x *= speedMultiplier * deltaTime;
            b.velocityVector.y *= speedMultiplier * deltaTime;
        }
        
        count = (count + 1) % max_bullets;
        start = steady_clock::now();   
    }  
}

void Bullet::checkForCollisions(Bullet* bullets, Invader* invaders, Player& P, Turret* turrets)
{
    for(int i = 0; i < max_bullets; i++)
    {
        Bullet& B = bullets[i];
        //if(B.currentlyDrawnOrNot == currentlyDrawnOrNot::OFF_SCREEN) continue;
        if(B.team == TEAM::PLAYER) 
        {
            checkForInvaderCollisions(bullets + i, invaders);
            checkForTurretCollisions(bullets + i, turrets);
            //checkForBulletCollisions(i, bullets);
        }
        else //if team == invader or turret bullet
        {
            checkForPlayerCollision(P, bullets + i, bullets[i].invaderRadius);
        }
    }
}

#if 0
void Bullet::checkForBulletCollisions(const int playerBullet, Bullet* bullets)
{
    Bullet& PB = bullets[playerBullet];
    for(int i=0; i<max_bullets;i++)
    {
        Bullet& B = bullets[i];
        if(i == playerBullet)continue;
        if(isColliding(B.pos, PB.pos, B.invaderRadius, PB.playerRadius) &&
           B.currentlyDrawnOrNot == currentlyDrawnOrNot::ON_SCREEN &&
           PB.currentlyDrawnOrNot == currentlyDrawnOrNot::ON_SCREEN)
        {
            B.currentlyDrawnOrNot = currentlyDrawnOrNot::OFF_SCREEN;
            PB.currentlyDrawnOrNot = currentlyDrawnOrNot::OFF_SCREEN;
        }
    }
}
#endif

void Bullet::checkForPlayerCollision(Player& P, Bullet* B, const float radius)
{
    Vector2 bc = B->pos;
    
    if(B->currentlyDrawnOrNot == currentlyDrawnOrNot::OFF_SCREEN)return;

    if(Utils::isColliding(bc, P.circle1, radius, P.radius1)||
       Utils::isColliding(bc, P.circle2, radius, P.radius1)||
       Utils::isColliding(bc, P.circle3, radius, P.radius1)||
       Utils::isColliding(bc, P.smallcircle, radius, P.smallradius))
    {
        B->currentlyDrawnOrNot = currentlyDrawnOrNot::OFF_SCREEN;
        Utils::PcollisionResolution(P);
    }
}

void Bullet::checkForTurretCollisions(Bullet* B, Turret* turrets)
{
    for(int i=0; i<Turret::turretCount; i++)
    {
        Turret& T = turrets[i];

        if(B->currentlyDrawnOrNot == currentlyDrawnOrNot::OFF_SCREEN)continue;

        if(Utils::isColliding(B->pos, T.hitCircle, B->playerRadius, T.radius))
        {
            B->currentlyDrawnOrNot = currentlyDrawnOrNot::OFF_SCREEN;
            Utils::TcollisionResolution(i, turrets[i]);
        }
    }
}

void Bullet::checkForInvaderCollisions(Bullet* B, Invader* invaders)
{
    for(int i=0; i<Invader::max_invaders; i++)
    {
        Invader& I = invaders[i];

        if(B->currentlyDrawnOrNot == currentlyDrawnOrNot::OFF_SCREEN || !(Invader::validInvaders & 1<<i))continue;

        if(Utils::isColliding(B->pos, I.centerCircle, B->playerRadius, I.centerRadius)||
           Utils::isColliding(B->pos, I.leftCircle, B->playerRadius, I.leftRightRadius)||
           Utils::isColliding(B->pos, I.rightCircle, B->playerRadius, I.leftRightRadius))
        {
            B->currentlyDrawnOrNot = currentlyDrawnOrNot::OFF_SCREEN;
            Utils::IcollisionResolution(invaders[i], i);          
        }
    }
}

void Bullet::drawBullets(Bullet* bullets, float dynamicBulletVelocity, int screenHeight, Player& player)
{
    for(int i = 0; i < max_bullets; i++)
    {
        Bullet& B = bullets[i];

        if( (B.pos.y >= screenHeight) || (B.pos.y <= -55) ) 
        {
            bullets[i].currentlyDrawnOrNot = currentlyDrawnOrNot::OFF_SCREEN;
            continue;
        }
        if(B.team == TEAM::INVADER && B.currentlyDrawnOrNot == currentlyDrawnOrNot::ON_SCREEN)
        {
            bullets[i].pos.y += dynamicBulletVelocity;
            Vector2 drawPos = {B.pos.x - B.textureOffset.x, 
                               B.pos.y - B.textureOffset.y};
            DrawTextureEx(redLaser, drawPos, NULL, Iscale, WHITE);
            //DrawCircleLines(B.pos.x, B.pos.y, B.invaderRadius, BLUE);
        }
        else if(B.team == TEAM::PLAYER && B.currentlyDrawnOrNot == currentlyDrawnOrNot::ON_SCREEN)
        {
            B.pos.y -= dynamicBulletVelocity;
            Vector2 drawPos = {B.pos.x - B.textureOffset.x, 
                               B.pos.y - B.textureOffset.y};
            DrawTextureEx(greenLaser, drawPos, NULL, Tscale, WHITE);
            //DrawCircleLines(B.pos.x, B.pos.y, 6, ORANGE);
        }
        else if (B.team == TEAM::TURRET && B.currentlyDrawnOrNot == currentlyDrawnOrNot::ON_SCREEN)
        {
            B.pos.x += B.velocityVector.x;
            B.pos.y += B.velocityVector.y;

            B.rotatedOffset.x += B.velocityVector.x;
            B.rotatedOffset.y += B.velocityVector.y;

            DrawTextureEx(turretLaser, B.rotatedOffset, B.angleDegrees, Tscale, WHITE);
            //DrawCircleLines(B.pos.x, B.pos.y, B.turretRadius, BLUE);
        }
    }
}