#pragma once
#include <raylib.h>

class Invader
{
public:

    Vector2 pos;
    float wingRadiusLarge;// wingRadiusSmall, noseRadius;
    float noseTipRadius;
    Vector2 center_wingLargeLeft, center_wingLargeRight;
    Vector2 noseTipCircleCenter;
  
    static Texture2D invaderTexture;
    const static int max_invaders;
    const static float scale;
    static float speedMultiplier;
    static const float pixelsToDrop;

    Invader(Vector2 pos);

    static void initInvader_HitCircleLocations(Invader* invaders, const int index);
    static Invader* initInvaders(const int screenHeight, const int screenWidth);
    static void updateInvaders(Invader* invaders, const int screenWidth, bool& direction, double dynamicVelocity);
    static void drawInvaders(Invader* invaders);
    static float clampAndLowerInvaders(float val, float screenWidth, bool& direction, Invader* invaders);
    static void lowerInvadersAndHitCircles(Invader* invaders);
    inline static void updateInvaderHitCircles(Invader* invaders, const int index);
};