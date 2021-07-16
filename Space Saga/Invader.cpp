#include "Invader.h"
#include "Colision.h"


#include <iostream>


const int Invader::max_invaders = 12;
Texture2D Invader::invaderTexture;
const float Invader::scale = 0.4f;
float Invader::speedMultiplier = 40.0f;
const float Invader::pixelsToDrop = 8.0f;

Invader::Invader(Vector2 pos)
    : pos(pos), wingRadiusLarge(7.0f), noseTipRadius(3.0f)
{
}

Invader* Invader::initInvaders(const int screenHeight, const int screenWidth)
{
    invaderTexture = LoadTexture("Resources/Textures/enemyShip.png");

    //how many total pixels that arent ships on x axis                           
    float pixelsBetweenShips = screenWidth - ((invaderTexture.width * scale) * max_invaders);
    pixelsBetweenShips /= max_invaders + 1;//divided by numof spaces bewteen ships

    Invader* invaders = (Invader*)malloc(sizeof(Invader) * max_invaders);//mem allocate 1D array for all rows of invaders

    for(int i = 0; i < max_invaders; ++i)
    {
        invaders[i] = Invader({i * (invaderTexture.width * scale), 120});
        invaders[i].pos.x += pixelsBetweenShips * (i + 1);
        initInvader_HitCircleLocations(invaders, i);
    }
    return invaders;
}

void Invader::initInvader_HitCircleLocations(Invader* invaders, const int index)
{
    invaders[index].center_wingLargeLeft.x = invaders[index].pos.x + 12.0;
    invaders[index].center_wingLargeLeft.y = invaders[index].pos.y + 22.0;

    invaders[index].center_wingLargeRight.x = invaders[index].pos.x + 31.0;
    invaders[index].center_wingLargeRight.y = invaders[index].pos.y + 22.0;

    invaders[index].noseTipCircleCenter.x = invaders[index].pos.x + 22.0f;
    invaders[index].noseTipCircleCenter.y = invaders[index].pos.y + 80.0f;
}

void Invader::updateInvaders(Invader* invaders, const int screenWidth, bool& direction, double dynamicVelocity)
{
    for(int i = 0; i < max_invaders; i++)
    { 
        invaders[i].pos.x = clampAndLowerInvaders(invaders[i].pos.x, (float)screenWidth - invaderTexture.width * scale, direction, invaders);    
               
        if(direction == 0) //move invaders left
        {
            invaders[i].pos.x -= dynamicVelocity;
            updateInvaderHitCircles(invaders, i);
        }
        else //if direction is right move right
        {
            invaders[i].pos.x += dynamicVelocity;
            updateInvaderHitCircles(invaders, i);
        }
    }
}

void Invader::drawInvaders(Invader* invaders)
{
    for(int i = 0; i < max_invaders; i++)
    {
        DrawTextureEx(invaderTexture, invaders[i].pos, NULL, scale, WHITE);
        //drawInvadersCircleTest(invaders, i);
    }
}

float Invader::clampAndLowerInvaders(float val, float screenWidth, bool& direction, Invader* invaders)
{
    if(val < 0)
    {
        direction ^= 1; //change the direction
        lowerInvadersAndHitCircles(invaders);
        return 0.0f;
    }
    if(val > screenWidth)
    {
        direction ^= 1; //change the direction
        lowerInvadersAndHitCircles(invaders);
        return screenWidth;
    }
    return val;
}

void Invader::lowerInvadersAndHitCircles(Invader* invaders)
{
    for(int i = 0; i < max_invaders; i++)
    {
        invaders[i].pos.y += pixelsToDrop;
        invaders[i].center_wingLargeLeft.y += pixelsToDrop;
        invaders[i].center_wingLargeRight.y += pixelsToDrop;
        invaders[i].noseTipCircleCenter.y += pixelsToDrop;
    }
}

inline void Invader::updateInvaderHitCircles(Invader* invaders, const int index)
{
    invaders[index].center_wingLargeLeft.x = invaders[index].pos.x + 12.0;
    invaders[index].center_wingLargeRight.x = invaders[index].pos.x + 31.0;
    invaders[index].noseTipCircleCenter.x = invaders[index].pos.x + 22.0f;
}