#pragma once
#include <raylib.h>

class Bullet;

class Player
{
public:

    Vector2 pos;
    Texture2D texture;
    Vector2 textureOffset;
    int lives;

    //////////////////////////////////
    Vector2 circle1;   
    Vector2 circle2;   
    Vector2 circle3;
    Vector2 smallcircle;
    float radius1;
    float smallradius;       
    //////////////////////////////////

    static float speedMultiplier;
    static float scale;

    Player(const int screenWidth, const int screenHeight);
    //updatePlayer every game loop to update players position and whatnot
    void initTextureStuff();
    void updatePlayer(const int screenWidth, float dynamicPlayerVelocity, Bullet*);
    void drawPlayer();
};