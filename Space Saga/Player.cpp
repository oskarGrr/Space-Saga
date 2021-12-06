#include "Player.h"
#include "Bullet.h"
#include <raylib.h>
#include <chrono>

// speedConstant will get multipled with the deltatime to get a dynamic velocity (to tell how many units player should move per frame)
// scale scales the texture size
float Player::scale = 0.8f;
float Player::speedMultiplier = 420.0f;

Player::Player(const int SW, const int SH)
    : pos({ (float)SW * 0.5f , (float)SH * 0.85f }), lives(3)
{
    radius1 = 16.0f;
    circle1 = {pos.x, pos.y};   
    smallcircle = {pos.x, pos.y - 30.0f};
    smallradius = 5.0f;
}

void Player::initTextureStuff()
{
    texture = LoadTexture("Resources/Textures/arwing.png");
    textureOffset = {texture.width * scale / 2, texture.height * scale / 2};
    circle2 = { pos.x - textureOffset.x * 0.5f, pos.y + 20.0f };
    circle3 = { pos.x + textureOffset.x * 0.5f, pos.y + 20.0f };
}

void Player::updatePlayer(const int SW, float dynamicPlayerVelocity, Bullet* bullets)
{
    if(IsKeyDown(KeyboardKey::KEY_RIGHT) || IsKeyDown(KeyboardKey::KEY_D))
    {
        if(pos.x + textureOffset.x + dynamicPlayerVelocity < SW)
        {
            pos.x     += dynamicPlayerVelocity;
            circle1.x += dynamicPlayerVelocity;
            circle2.x += dynamicPlayerVelocity;
            circle3.x += dynamicPlayerVelocity;
            smallcircle.x += dynamicPlayerVelocity;
        }
    }
    
    if(IsKeyDown(KeyboardKey::KEY_LEFT) || IsKeyDown(KeyboardKey::KEY_A))
    {
        if(pos.x - textureOffset.x - dynamicPlayerVelocity > 0.0f)
        {
            pos.x     -= dynamicPlayerVelocity;       
            circle1.x -= dynamicPlayerVelocity;
            circle2.x -= dynamicPlayerVelocity;
            circle3.x -= dynamicPlayerVelocity;
            smallcircle.x -= dynamicPlayerVelocity;
        }
    }
}

void Player::drawPlayer()
{
    Vector2 drawPos;
    drawPos.x = pos.x - textureOffset.x;
    drawPos.y = pos.y - textureOffset.y;
    DrawTextureEx(texture, drawPos, NULL, scale, WHITE);
}