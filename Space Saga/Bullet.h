#pragma once
#include <raylib.h>
#include <chrono>

class Player;
class Invader;
class Turret;

class Bullet
{
public:

    enum class TEAM : char
    {
        PLAYER = 1, INVADER = 0, TURRET = -1
    };//turret and invader obv same team. Just enum used for telling if i should rotate bullut or not
    enum class currentlyDrawnOrNot : bool 
    {
        ON_SCREEN = 1, OFF_SCREEN = 0
    };
    union 
    {
        float playerRadius; 
        float turretRadius;
        float invaderRadius;
    };

    Vector2 pos;
    TEAM team;
    currentlyDrawnOrNot currentlyDrawnOrNot;
    Vector2 velocityVector;
    float angleDegrees;
    Vector2 rotatedOffset;
    Vector2 textureOffset;
    
    static int max_bullets;
    static int count;
    static float Tscale;
    static float Iscale;
    static float speedMultiplier;
    static Texture2D greenLaser;
    static Texture2D redLaser;
    static Texture2D turretLaser;
    
    Bullet(Vector2 InitialPos, TEAM whichTeam);

    static void manageBullets(Bullet * bullets, Invader * invaders, Player& player, Turret* turrets, float deltaTime);
    static void updatePlayerBullets(Bullet* bullets, Vector2 pos);
    static void updateInvaderBullets(Invader* invaders, Bullet* bullets);
    static void updateTurretBullets(Bullet* bullets, Turret* turrets, Player& player, float deltaTime);
    static void drawBullets(Bullet* bullets, float dynamicVelocity, int screenHeight, Player&);
    static void init_Bullet_Textures();
    static void checkForCollisions(Bullet* bullets, Invader* invaders, Player& player, Turret* turrets);
    static void checkForBulletCollisions(const int playerBullet, Bullet* bullets);
    static void checkForPlayerCollision(Player& P, Bullet* bullets, const float radius);
    static void checkForTurretCollisions(Bullet* B, Turret*);
    static void checkForInvaderCollisions(Bullet* bullets, Invader* invaders);
};