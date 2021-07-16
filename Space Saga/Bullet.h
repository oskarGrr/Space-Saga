#pragma once
#include <raylib.h>
#include <chrono>

class Player;
class Invader;

class Bullet
{
public:
    enum TEAM : char
    {
        PLAYER = 1, INVADER = 0, TURRET = -1
    };
    enum currentlyDrawnOrNot : bool 
    {
        ON_SCREEN = 1, OFF_SCREEN = 0
    };

    Vector2 pos;
    float radius;    
    TEAM team;
    currentlyDrawnOrNot currentlyDrawnOrNot;
    Vector2 frontCircleCenter;

    static int max_bullets;
    static int count;
    static float Gscale;
    static float Rscale;
    static float speedMultiplier;
    static Texture2D greenLaser;
    static Texture2D redLaser;
    static Texture2D turretLaser;
    
    Bullet(Vector2 InitialPos, TEAM whichTeam);

    static void manageBullets(TEAM team, Vector2 shipPos, Bullet* bullets, 
                              std::chrono::duration<float>& timeElapsed,
                              std::chrono::steady_clock::time_point& start,
                              std::chrono::steady_clock::time_point& end, bool& hasSpaceBeenPressedYet,
                              std::chrono::steady_clock::time_point& startI,
                              std::chrono::steady_clock::time_point& endI,
                              std::chrono::duration<float>& timeElapsedI,
                              Invader* invaders
                              );

    static void updatePlayerBullets();
    static void updateInvaderBullets();
    static void updateTurretBullets();

    static void drawBullets(Bullet* bullets, double dynamicVelocity, int screenHeight, float playerWidth);
    static void InitHitCirlceLocations(Bullet* bullets, const int index);
    static inline Vector2 allignLaserWithPlayer(float playerWidth, Bullet* bullets, int index);
    static Vector2 allignLaserWithInvader(Bullet* bullets, const int index);
    static void init_Bullet_Resources();
    static void checkForCollisions(Bullet* bullets, Invader* invaders, Player& player);
};

/*static void manageBullets(TEAM team, Vector2 shipPos, Bullet* bullets, 
                              std::chrono::duration<float>& timeElapsed,
                              std::chrono::steady_clock::time_point& start,
                              std::chrono::steady_clock::time_point& end, bool& hasSpaceBeenPressedYet,
                              std::chrono::steady_clock::time_point& startI,
                              std::chrono::steady_clock::time_point& endI,
                              std::chrono::duration<float>& timeElapsedI,
                              Invader* invaders
                              );
                              */