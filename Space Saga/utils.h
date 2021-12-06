#pragma once
#include <raylib.h>
#include "player.h"

class Invader;
//class Player;
class Turret;
class Bullet;

enum class GameState : unsigned int 
{
    PLAYING, PLAYERDIED, GAMEWON
};

class Utils
{
public:

    static Texture2D backGround2;
    static const float scale;
    static Texture2D explosionTexture;
    static bool runExplosionAnimation;
    static const int screenWidth;
    static const int screenHeight;
    static GameState gameState;
    static Player player;

    static Sound explSoundI;
    static Sound explSoundP;

    Font font;
    Turret*  turrets;
    Invader* invaders;
    Bullet*  bullets;
    
    Utils(const Utils&) = delete;
    Utils& operator = (const Utils&) = delete;

    Utils();
   ~Utils();
    
    static void drawUI(const int SW, const int SH, Player& P, Font f);
    static bool isColliding(Vector2 p1, Vector2 p2, float r1, float r2);
    static Vector2 normalizeVector2(Vector2 v);
    static Vector2 rotateVector(Vector2 pos, float radians);
    static Vector2 rotateAroundSpecificPoint(Vector2 pos, Vector2 fulcrum, float radians);
    static void IcollisionResolution(Invader&, const int index);
    static void TcollisionResolution(const int index, Turret&);
    static void PcollisionResolution(Player&);
    static void explosionAnimation(Vector2 pos, bool);
    static void debug(Invader* invaders, Turret* turrets, Player& player);
    static bool isMouseOver(const Rectangle& rec);
    static void initExplosionTexture();
};