#pragma once
#include <raylib.h>

class Player
{
public:

    float x_pos;
    float Y_pos;
    Texture2D shipTexture;
    float playerWidth;
    //////////////////////////////////
    float   rearNoseRadius;
    Vector2 rearNoseCircleCenter;
    float   frontNoseRadius;
    Vector2 frontNoseCircleCenter;
    float   largeWingRadius;
    Vector2 largeWingCircleCenterLeft;
    Vector2 largeWingCircleCenterRight;
    float   smallWingRadius;
    Vector2 smallWingLeftCircleCenter;
    Vector2 smallWingRightCircleCenter;
    //////////////////////////////////

    static double speedMultiplier;
    static float scale;

    Player(const int screenWidth, const int screenHeight);
    //updatePlayer every game loop to update players position and whatnot
    void updatePlayer(const int screenWidth, const int screenHeight, double dynamicPlayerVelocity);
};