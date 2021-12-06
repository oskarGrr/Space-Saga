#pragma once
#include <raylib.h>

class Invader
{
public:

    enum class invaderType : unsigned int
    {
        REAR, MIDDLE, FRONT
    };

    Vector2 pos;
    invaderType type;
    Vector2 centerCircle;
    Vector2 leftCircle;
    Vector2 rightCircle;
    const Texture2D* whichTexture;
    const Vector2* whichTextureOffset;
    const float* whichScale;

    static float centerRadius;
    static float leftRightRadius;

    static Texture2D Texture1;
    static Texture2D Texture2;
    static Texture2D Texture3;
    static Vector2 textureOffset1;
    static Vector2 textureOffset2;
    static Vector2 textureOffset3;

    static const int type1Count;
    static const int type2Count;
    static const int type3Count;
    static int max_invaders;
    static int invaderCount;
    static int validInvaders;

    static const float scale1;
    static const float scale2;
    static const float scale3;

    static float speedMultiplier;
    static float maxSpeed;
    static float shootSpeed;
    static float shootSpeedIncreaseAmount;
    static float speedIncreaseAmount;
    static const float pixelsToDrop;


    Invader(Vector2 pos, invaderType type);

    static void clamp(const int SW, const float dynamicVel, bool& direction, 
                      Invader& minInvader, Invader& maxInvader, bool& shouldLower);
    static int findBiggestXpos(Invader*);
    static int findSmallestXpos(Invader* invaders, const int SW);
    static void initInvaders(const int , const int , Invader*);
    static void updateInvaders(const int SW, const int SH, const float dynamicVel, Invader* invaders);
    static void lowerInvaders(Invader* invaders, const float dynamicVel, const bool direction, bool& shouldLower);
    static void drawInvaders(Invader* invaders);
};