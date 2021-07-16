#include "Bullet.h"
#include "Turret.h"
#include "player.h"
#include "Colision.h"
#include <chrono>


#include <iostream>


int Bullet::count;
int Bullet::max_bullets = 16;
float Bullet::speedMultiplier = 400.0f;
float Bullet::Gscale = 0.6f;
float Bullet::Rscale = 1.5f;
Texture2D Bullet::greenLaser;
Texture2D Bullet::redLaser;
Texture2D Bullet::turretLaser;

void Bullet::init_Bullet_Resources()
{
    greenLaser = LoadTexture("Resources/Textures/greenLaser.png");
    redLaser = LoadTexture("Resources/Textures/redLaser.png");
    turretLaser = LoadTexture("Resources/Textures/turretLaser.png");
}

Bullet::Bullet(Vector2 InitialPos, TEAM team)//offset x and y need to be manually changed for now
    : pos({InitialPos.x, InitialPos.y}), radius(8.0f), 
      team(team), currentlyDrawnOrNot(OFF_SCREEN)
{}

void Bullet::InitHitCirlceLocations(Bullet* bullets, const int index)
{
    if(bullets[index].team == INVADER) bullets[index].frontCircleCenter = {bullets[index].pos.x + 24.0f, bullets[index].pos.y + 37.0f};
    else /*if team == player*/ bullets[index].frontCircleCenter = {bullets[index].pos.x + 55.0f, bullets[index].pos.y + 75.0f};       
}

void Bullet::manageBullets(TEAM team, Vector2 shipPos, Bullet* bullets,
                           std::chrono::duration<float>& timeElapsedP,
                           std::chrono::steady_clock::time_point& startP,
                           std::chrono::steady_clock::time_point& endP,
                           bool& hasSpaceBeenPressedYet,
                           std::chrono::steady_clock::time_point& startI,
                           std::chrono::steady_clock::time_point& endI,
                           std::chrono::duration<float>& timeElapsedI,
                           Invader* invaders
                           )
{
    if(team == PLAYER)
    {
        if(hasSpaceBeenPressedYet)
        {
            endP = std::chrono::steady_clock::now();
            timeElapsedP = endP - startP;
            if(timeElapsedP.count() > 0.8f)
            {//if space has been pressed already and the time inbeween shots is greater than 1 second
                startP = std::chrono::steady_clock::now();
                bullets[count] = Bullet(shipPos, team);
                bullets[count].currentlyDrawnOrNot = ON_SCREEN;
                InitHitCirlceLocations(bullets, count);
                count = (count + 1) % max_bullets;
            }
            return;
        }
        {//only goes here on first space press
            startP = std::chrono::steady_clock::now();

            bullets[count] = Bullet(shipPos, PLAYER);
            bullets[count].currentlyDrawnOrNot = ON_SCREEN;
            InitHitCirlceLocations(bullets, count);
            count = (count + 1) % max_bullets;
            hasSpaceBeenPressedYet = true;
        }
    }
    else //if team is invader
    {
        if(timeElapsedI != std::chrono::steady_clock::duration::zero())
        {
            endI = std::chrono::steady_clock::now();
            timeElapsedI = endI - startI; 
            if(timeElapsedI.count() > 0.8f)
            {
                startI = std::chrono::steady_clock::now();
                bullets[count] = Bullet(invaders[rand() % Invader::max_invaders].pos, INVADER);
                bullets[count].currentlyDrawnOrNot = ON_SCREEN;
                InitHitCirlceLocations(bullets, count);
                count = (count + 1) % max_bullets;
            }
        }
        else //if first shot from invaders then start timer and make bullet
        {
            timeElapsedI = endI - startI;
            bullets[count] = Bullet(invaders[rand() % Invader::max_invaders].pos, INVADER);
            bullets[count].currentlyDrawnOrNot = ON_SCREEN;
            InitHitCirlceLocations(bullets, count);
            count = (count + 1) % max_bullets;
            startI = std::chrono::steady_clock::now();
            timeElapsedI = endI - startI;
        }
    }
}

void Bullet::updatePlayerBullets()
{
}

void Bullet::updateInvaderBullets()
{
}

void Bullet::updateTurretBullets()
{
}

void Bullet::checkForCollisions(Bullet* bullets, Invader* invaders, Player& player)
{
    for(int i = 0; i < max_bullets; ++i)
    {
        if(bullets[i].currentlyDrawnOrNot == ON_SCREEN) 
        {
            if(bullets[i].team == PLAYER)
            {
                for(int j = 0; j < Invader::max_invaders; ++j)
                {
                    if(isColliding(bullets[i].frontCircleCenter, invaders[j].center_wingLargeLeft, bullets[i].radius, invaders[j].wingRadiusLarge)
                        ||
                       isColliding(bullets[i].frontCircleCenter, invaders[j].center_wingLargeRight, bullets[i].radius, invaders[j].wingRadiusLarge)
                        ||
                       isColliding(bullets[i].frontCircleCenter, invaders[j].noseTipCircleCenter, bullets[i].radius, invaders[j].noseTipRadius))
                    {
                        std::cout << "COLLISION" << "\n";
                        bullets[i].currentlyDrawnOrNot = OFF_SCREEN;
                    }
                }
            }//if team == player
            else if(isColliding(bullets[i].frontCircleCenter, player.frontNoseCircleCenter, bullets[i].radius, player.frontNoseRadius)
                     ||
                    isColliding(bullets[i].frontCircleCenter, player.rearNoseCircleCenter, bullets[i].radius, player.rearNoseRadius)
                     ||
                    isColliding(bullets[i].frontCircleCenter, player.largeWingCircleCenterLeft, bullets[i].radius, player.largeWingRadius)
                     ||
                    isColliding(bullets[i].frontCircleCenter, player.largeWingCircleCenterRight, bullets[i].radius, player.largeWingRadius)
                     ||
                    isColliding(bullets[i].frontCircleCenter, player.smallWingLeftCircleCenter, bullets[i].radius, player.smallWingRadius)
                     ||
                    isColliding(bullets[i].frontCircleCenter, player.smallWingRightCircleCenter, bullets[i].radius, player.smallWingRadius))
            {
                std::cout << "COLLISION" << "\n";
                bullets[i].currentlyDrawnOrNot = OFF_SCREEN;
            }
        }
    }
}

void Bullet::drawBullets(Bullet* bullets, double dynamicBulletVelocity, int screenHeight, float playerWidth)
{
    for(int i = 0; i < max_bullets; i++)
    {
        if( (bullets[i].pos.y >= screenHeight) || (bullets[i].pos.y <= -55) ) 
        {
            bullets[i].currentlyDrawnOrNot = OFF_SCREEN;
            continue;
        }
        if(bullets[i].team == INVADER)
        {
            bullets[i].pos.y += dynamicBulletVelocity;
            bullets[i].frontCircleCenter.y = bullets[i].pos.y + 80.0f;
            DrawTextureEx(redLaser, allignLaserWithInvader(bullets, i), NULL, Rscale, WHITE);
            //DrawCircleLines(bullets[i].frontCircleCenter.x, bullets[i].frontCircleCenter.y, bullets[i].radius, BLUE);
        }
        else
        {
            bullets[i].pos.y -= dynamicBulletVelocity;
            bullets[i].frontCircleCenter.y = bullets[i].pos.y;
            DrawTextureEx(greenLaser, allignLaserWithPlayer(playerWidth, bullets, i), NULL, Bullet::Gscale, WHITE);
            //DrawCircleLines(bullets[i].frontCircleCenter.x, bullets[i].frontCircleCenter.y, bullets[i].radius, RED);
        }
    }
}

inline Vector2 Bullet::allignLaserWithPlayer(float playerWidth, Bullet* bullets, int index)
{return {bullets[index].pos.x + playerWidth * 0.5f - greenLaser.width * Gscale * 0.5f, bullets[index].pos.y - 22};}

inline Vector2 Bullet::allignLaserWithInvader(Bullet* bullets, const int index)
{return {bullets[index].pos.x + 4.0f, bullets[index].pos.y + 50.0f};}