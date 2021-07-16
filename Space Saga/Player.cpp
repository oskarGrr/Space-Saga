#include "Player.h"
#include "Bullet.h"
#include <raylib.h>

// speedConstant will get multipled with the deltatime to get a dynamic velocity (to tell how many units player should move per frame)
// scale scales the texture size
float Player::scale = 0.8f;
double Player::speedMultiplier = 380.0f;

Player::Player(const int screenWidth, const int screenHeight)
    : x_pos(screenWidth * 0.5f), Y_pos(screenHeight * 0.85f),
      shipTexture(LoadTexture("Resources/Textures/arwing.png")),
      playerWidth(shipTexture.width * scale),
      rearNoseRadius(8.0f), rearNoseCircleCenter({x_pos + 55.0f, Y_pos + 30.0f}),
      frontNoseRadius(4.0f), frontNoseCircleCenter({x_pos + 55.0f, Y_pos + 12.0f}),
      largeWingRadius(10.0f), largeWingCircleCenterLeft({x_pos + 40.0f, Y_pos + 53.0f}),
      largeWingCircleCenterRight({x_pos + 71.0f ,Y_pos + 53.0f}) , smallWingRadius(9.0f),
      smallWingLeftCircleCenter({x_pos + 16.0f, Y_pos + 67.0f}), smallWingRightCircleCenter({x_pos + 95.0f, Y_pos + 67.0f})
{}

inline float clamp(float value, float min, float max)
{
    if      (value < min) return min;
    else if (value > max) return max;
    else return value;
}

void Player::updatePlayer(const int screenWidth, const int screenHeight, double dynamicPlayerVelocity)
{
    if (IsKeyDown(KeyboardKey::KEY_RIGHT) || IsKeyDown(KeyboardKey::KEY_D))
    { 
        x_pos += dynamicPlayerVelocity;

        frontNoseCircleCenter.x = x_pos + 55.0f;
        rearNoseCircleCenter.x = x_pos + 55.0f;
        largeWingCircleCenterLeft.x = x_pos + 40.0f;
        largeWingCircleCenterRight.x = x_pos + 71.0f;
        smallWingLeftCircleCenter.x = x_pos + 16.0f;
        smallWingRightCircleCenter.x = x_pos + 95.0f;
    }
    if (IsKeyDown(KeyboardKey::KEY_LEFT) || IsKeyDown(KeyboardKey::KEY_A))
    { 
        x_pos -= dynamicPlayerVelocity;

        frontNoseCircleCenter.x = x_pos + 55.0f;
        rearNoseCircleCenter.x = x_pos + 55.0f;
        largeWingCircleCenterLeft.x = x_pos + 40.0f;
        largeWingCircleCenterRight.x = x_pos + 71.0f;
        smallWingLeftCircleCenter.x = x_pos + 16.0f;
        smallWingRightCircleCenter.x = x_pos + 95.0f;
    }

    x_pos = clamp(x_pos, 0, (float)screenWidth - playerWidth); // Bounds check       
    DrawTextureEx(shipTexture, {x_pos, Y_pos}, NULL, scale, WHITE);

    //DrawCircleLines(rearNoseCircleCenter.x, rearNoseCircleCenter.y, rearNoseRadius, BLACK);
   // DrawCircleLines(frontNoseCircleCenter.x, frontNoseCircleCenter.y, frontNoseRadius, ORANGE);
    //DrawCircleLines(largeWingCircleCenterLeft.x, largeWingCircleCenterLeft.y, largeWingRadius, ORANGE);
   // DrawCircleLines(largeWingCircleCenterRight.x, largeWingCircleCenterRight.y, largeWingRadius, ORANGE);
   // DrawCircleLines(smallWingLeftCircleCenter.x, smallWingLeftCircleCenter.y, smallWingRadius, ORANGE);
   // DrawCircleLines(smallWingRightCircleCenter.x, smallWingRightCircleCenter.y, smallWingRadius, ORANGE);
}