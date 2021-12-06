#include "Invader.h"
#include "utils.h"
#include <cmath>
#include <chrono>
#include <iostream>

Vector2 Invader::textureOffset1;
Vector2 Invader::textureOffset2;
Vector2 Invader::textureOffset3;

float Invader::centerRadius = 8.0f;
float Invader::leftRightRadius = 9.0f;

const int Invader::type1Count = 8;
const int Invader::type2Count = 7;
const int Invader::type3Count = 6;
int Invader::max_invaders = type1Count + type2Count + type3Count;
int Invader::invaderCount = max_invaders;

//results in a binary representation of 1s that coorespond to the invaders that are alive
//used to tell which ones are alive with bitwise operations instead of using array of bool
int Invader::validInvaders = pow(2, max_invaders) - 1;

//used for knowing which invaders to skip over drawing
//the binary 2s place coorisponding to the invader will be turned off 
//if it has exploded

Texture2D Invader::Texture1;
Texture2D Invader::Texture2;
Texture2D Invader::Texture3;

const float Invader::scale1 = 0.28f;
const float Invader::scale2 = 0.25f;
const float Invader::scale3 = 0.25f;

float Invader::speedMultiplier = 30.0f;
float Invader::speedIncreaseAmount = 22.0f;

float Invader::shootSpeed = 0.8f;
float Invader::shootSpeedIncreaseAmount = 0.033333f;

const float Invader::pixelsToDrop = 22.0f;

Invader::Invader(Vector2 pos, invaderType type)
    : pos(pos), type(type), centerCircle(pos)
{
    if(type == invaderType::FRONT)
    {
        leftCircle = this->pos;
   
        leftCircle.x -= 15.0f;

        rightCircle = this->pos;
    
        rightCircle.x += 15.0f;

        whichScale = &scale3;
        whichTexture = &Texture3;
        whichTextureOffset = &textureOffset3;
    }
    else if(type == invaderType::MIDDLE)
    {
        leftCircle = this->pos;
        leftCircle.x -= 12.0f;
        
        rightCircle = this->pos;
        rightCircle.x += 12.0f;

        centerCircle.y += 10.0f;

        whichScale = &scale2;
        whichTexture = &Texture2;
        whichTextureOffset = &textureOffset2;
    }
    else//type is back row
    {
        leftCircle = this->pos;
        leftCircle.x -= 16.0f;
        
        rightCircle = this->pos;
        rightCircle.x += 16.0f;

        whichScale = &scale1;
        whichTexture = &Texture1;
        whichTextureOffset = &textureOffset1;
    }
}

void Invader::initInvaders(const int SW, const int SH, Invader* invaders)
{
    Texture1 = LoadTexture("Resources/Textures/invader_1.png");
    Texture2 = LoadTexture("Resources/Textures/invader_2.png");
    Texture3 = LoadTexture("Resources/Textures/invader_3.png");
    textureOffset1 = {Texture1.width * scale1 / 2, Texture1.height * scale1/ 2};
    textureOffset2 = {Texture2.width * scale2 / 2, Texture2.height * scale2/ 2};   
    textureOffset3 = {Texture3.width * scale2 / 2, Texture3.height * scale2/ 2};

    const float spaceBetweenT1 = SW / (type1Count + 1);
    const float spaceBetweenT2 = SW / (type2Count + 1);
    const float spaceBetweenT3 = SW / (type3Count + 1);
    const float rearYpos = 160.0f;
    const float middleYpos = rearYpos + Texture1.height * scale1 + 20.0f;
    const float frontYpos = middleYpos + Texture2.height * scale2 + 20.0f;
    
    for(int i=0; i < type1Count; i++)
    {
        invaders[i] = Invader({ (i+1) * spaceBetweenT1 , rearYpos}, invaderType::REAR);
    } 
    for(int i=type1Count, j=0; i < type1Count + type2Count; i++)  
    {
        invaders[i] = Invader({ (j+1) * spaceBetweenT2 , middleYpos}, invaderType::MIDDLE);
        j++;
    }
    for(int i=type1Count + type2Count, j=0; i < max_invaders; i++) 
    {
        invaders[i] = Invader({ (j+1) * spaceBetweenT3 , frontYpos}, invaderType::FRONT);
        j++;
    }
}

int Invader::findBiggestXpos(Invader* invaders)
{//returns which invader is most right
    int max = 0;
    int whichInvader = 0;

    for(int i = 0; i < max_invaders; i++)
    {    
        if(!(validInvaders & 1 << i))continue;
        if(invaders[i].pos.x > max) 
        {
            max = invaders[i].pos.x;  
            whichInvader = i;
        }
    }
    return whichInvader;
}

int Invader::findSmallestXpos(Invader* invaders, const int SW)
{//returns which invader is most left
    int min = SW;
    int whichInvader = 0;

    for(int i = 0; i < max_invaders; i++)
    {
        if(!(validInvaders & 1 << i))continue;
        if(invaders[i].pos.x < min) 
        {
            min = invaders[i].pos.x;
            whichInvader = i;
        }
    }
    return whichInvader;
}

void Invader::updateInvaders(const int SW, const int SH, const float dynamicVel, Invader* invaders)
{  
       
    int maxInvader = findBiggestXpos(invaders); 
    int minInvader = findSmallestXpos(invaders, SW);

    static bool direction = 1;//clamp changes direction and returns wether the lowering should start
    static bool shouldLower = false;
    clamp(SW, dynamicVel, direction, invaders[minInvader], invaders[maxInvader], shouldLower);
    
    for(int i=0; i < max_invaders; ++i)
    {
        Invader& inv = invaders[i];

        if(shouldLower)
        {
            //lowerInvaders takes shouldlower as reference changing it when its done lowering
            lowerInvaders(invaders, dynamicVel, direction, shouldLower);
            return;
        }
        if(!direction)
        {          
            inv.pos.x -= dynamicVel;
            inv.centerCircle.x -= dynamicVel;
            inv.leftCircle.x -= dynamicVel;
            inv.rightCircle.x -= dynamicVel;
        }
        else//direction is true, true is right
        {
            inv.pos.x += dynamicVel;
            inv.centerCircle.x += dynamicVel;
            inv.leftCircle.x += dynamicVel;
            inv.rightCircle.x += dynamicVel;
        }
    }

    {//set speed depending on amount of alive invaders;

    }
}

void Invader::lowerInvaders(Invader* invaders, float dynamicVel, const bool direction, bool& shouldLower)
{
    for(int i=0; i < max_invaders; i++)
    {
        if(!direction)
        {
            invaders[i].pos.y += dynamicVel;
            invaders[i].pos.x -= dynamicVel;
                    
            invaders[i].centerCircle.x -= dynamicVel;
            invaders[i].leftCircle.x -= dynamicVel;
            invaders[i].rightCircle.x -= dynamicVel;
                    
            invaders[i].centerCircle.y += dynamicVel;
            invaders[i].leftCircle.y += dynamicVel;
            invaders[i].rightCircle.y += dynamicVel;
        }            
        else      
        {   
            invaders[i].pos.y += dynamicVel;
            invaders[i].pos.x += dynamicVel;
                    
            invaders[i].centerCircle.x += dynamicVel;
            invaders[i].leftCircle.x += dynamicVel;
            invaders[i].rightCircle.x += dynamicVel;
                   
            invaders[i].centerCircle.y += dynamicVel;
            invaders[i].leftCircle.y += dynamicVel;
            invaders[i].rightCircle.y += dynamicVel;
        }
    }

    static float posTrack = 0;
    posTrack += dynamicVel;

    if(posTrack > pixelsToDrop) 
    {
        posTrack = 0.0f;
        shouldLower = false;
    }
}

void Invader::clamp(const int SW, float dynamicVel, 
                    bool& direction, Invader& minInvader, 
                    Invader& maxInvader, bool& shouldLower)
{
    int minTexture = minInvader.whichTexture->width * *(minInvader.whichScale) / 2;
    int maxTexture = maxInvader.whichTexture->width * *(maxInvader.whichScale) / 2;

    if(minInvader.pos.x - minTexture < 0 ||
       maxInvader.pos.x + maxTexture > SW) 
    {
        direction ^= 1;
        shouldLower ^= 1;
    }
}

void Invader::drawInvaders(Invader* invaders)
{
    for(int i = 0; i < max_invaders; ++i)
    {
        if (!(validInvaders & 1 << i))
            continue;

        Invader & inv = invaders[i];
        Vector2 offsetPos1 = {inv.pos.x - inv.textureOffset1.x, inv.pos.y - inv.textureOffset1.y};
        Vector2 offsetPos2 = {inv.pos.x - inv.textureOffset2.x, inv.pos.y - inv.textureOffset2.y};
        Vector2 offsetPos3 = {inv.pos.x - inv.textureOffset3.x, inv.pos.y - inv.textureOffset3.y};
       
        //checks if the invader is still alive if not then skip over it

        if(invaders[i].type == invaderType::REAR)
        {
            DrawTextureEx(Texture1, offsetPos1, NULL, scale1, WHITE);
        }
        else if(invaders[i].type == invaderType::MIDDLE)
        {
            DrawTextureEx(Texture2, offsetPos2, NULL, scale2, WHITE);
        }
        else //if front row type
        {
            DrawTextureEx(Texture3, offsetPos3, NULL, scale3, WHITE);
        }
    }
}