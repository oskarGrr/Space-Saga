#include <raylib.h>
#include <cmath>

#include "utils.h"
#include "Invader.h"
#include "Player.h"
#include "Turret.h"
#include "Bullet.h"

#include <iostream>

Sound Utils::explSoundP;
Texture2D Utils::backGround2;
Sound Utils::explSoundI;
const float Utils::scale = 0.5f;
Texture2D Utils::explosionTexture;
bool Utils::runExplosionAnimation = false;
const int Utils::screenHeight = 900, Utils::screenWidth = 1600;
GameState Utils::gameState = GameState::PLAYING;
Player Utils::player = Player(screenWidth, screenHeight);

void Utils::initExplosionTexture()
{
    
}

#if 0
inline void drawCooldownBar(Player& P, const int SW, const int SH)
{
    Vector2 pos = { P.pos.x, P.pos.y - P.textureOffset.y * P.scale };

}
#endif

inline static void drawLivesText(Player& P, const int SH, Font f)
{
    const char* t = TextFormat("LIVES: %d", P.lives);
    Vector2 pos = { 0.0f, SH - 24.0f };
    DrawTextEx(f, t, pos, 24, 5.0f, WHITE);
}

void Utils::drawUI(const int SW, const int SH, Player& P, Font f)
{
    DrawTexture(backGround2, 0,0, WHITE);
    {//explosion stuff
        if (runExplosionAnimation) explosionAnimation({}, false);
    }
    drawLivesText(P, SH, f);
}

inline static void winCond()
{
    if ((!Invader::validInvaders) && (!Turret::validTurrets)) Utils::gameState = GameState::GAMEWON;     
}

Utils::~Utils()
{
    UnloadTexture(Invader::Texture1);
    UnloadTexture(Invader::Texture2);
    UnloadTexture(Invader::Texture3);
    UnloadTexture(Turret::gunTurretTexture);
    UnloadTexture(Turret::baseTurretTexture);
    UnloadTexture(Bullet::redLaser);
    UnloadTexture(Bullet::greenLaser);
    UnloadTexture(Bullet::turretLaser);
    UnloadTexture(player.texture);
    free(invaders);
    free(turrets);
    free(bullets);
}

Utils::Utils() : font(LoadFont(""))
{
    //freed automatically in destructor
    turrets  = (Turret*) malloc(sizeof(Turret)  * Turret::turretCount);
    bullets  = (Bullet*) malloc(sizeof(Bullet)  * Bullet::max_bullets);
    invaders = (Invader*)malloc(sizeof(Invader) * Invader::max_invaders);

    if( !(bullets || turrets || invaders) ) exit(EXIT_FAILURE);

    Bullet::init_Bullet_Textures();
    Turret::initTurrets(screenWidth, screenHeight, turrets);
    Invader::initInvaders(screenWidth, screenHeight, invaders);
    player.initTextureStuff();
    backGround2 = LoadTexture("Resources/Textures/backGround2.png");

    InitAudioDevice();
    {    
        explosionTexture = LoadTexture("Resources/Textures/explosion.png");        
        explSoundI = LoadSound("Resources/Textures/player_bullet.wav");
        explSoundP = LoadSound("Resources/Textures/bomb1.wav");
        SetSoundVolume(explSoundI, 0.5f);
        SetSoundVolume(explSoundP, 0.7f);
    }
}

bool Utils::isMouseOver(const Rectangle& rec)
{
    const Vector2 mPos = GetMousePosition();
    return (mPos.x > rec.x &&
            mPos.y > rec.y &&
            mPos.x < rec.width + rec.x &&
            mPos.y < rec.height + rec.y);
}

bool Utils::isColliding(Vector2 p0, Vector2 p1, float r0, float r1)
{//just a simple circle over circle collision detection
    Vector2 length;
    float lengthSqrd;
    float radiusSum;
    float radiusSquared;

    length.x = p1.x - p0.x;
    length.y = p1.y - p0.y;
    lengthSqrd = (length.x * length.x) + (length.y * length.y);

    radiusSum = r0 + r1;
    radiusSquared = radiusSum * radiusSum;
    //squaring both radius and comparing sum to magnitude between 
    //them squared as to not call sqrt func every frame / iteratino of main game loop

    return (lengthSqrd < radiusSquared);
}

Vector2 Utils::normalizeVector2(Vector2 v)
{
    float magnitude;
    magnitude = sqrt((v.x * v.x) + (v.y * v.y));
    return {v.x / magnitude, v.y / magnitude};
}

Vector2 Utils::rotateVector(Vector2 vec, float radians)
{
    Vector2 rotatedVec;
    rotatedVec.x = (vec.x * cos(radians)) - (vec.y * sin(radians));
    rotatedVec.y = (vec.x * sin(radians)) + (vec.y * cos(radians));
    return rotatedVec;
}

Vector2 Utils::rotateAroundSpecificPoint(Vector2 pos, Vector2 fulcrum, float radians)
{
    Vector2 posRelativeToOrigin;
    posRelativeToOrigin.x = pos.x - fulcrum.x;
    posRelativeToOrigin.y = pos.y - fulcrum.y;

    Vector2 rotatedPos;
    rotatedPos.x = (posRelativeToOrigin.x * cos(radians)) - (posRelativeToOrigin.y * sin(radians));
    rotatedPos.y = (posRelativeToOrigin.x * sin(radians)) + (posRelativeToOrigin.y * cos(radians));

    return {fulcrum.x + rotatedPos.x, fulcrum.y + rotatedPos.y};
}

void Utils::IcollisionResolution(Invader& I, const int index)
{
    {//start new explosion
        runExplosionAnimation = true;
        Vector2 explPos = 
        {
            explPos.x = I.pos.x - I.whichTextureOffset->x,
            explPos.y = I.pos.y - I.whichTextureOffset->y
        };
        explosionAnimation(explPos, true);
    }
    PlaySound(explSoundI);

    {
        Invader::validInvaders &= ~(1 << index);//turns off the bit related to the destroyed invader   
        Invader::invaderCount--;
        Invader::speedMultiplier *= 1.18f;
        Invader::shootSpeed += Invader::shootSpeedIncreaseAmount;
    }
    winCond();//check for win after invader death
}

void Utils::TcollisionResolution(const int index, Turret& T)
{
    {
        Vector2 explPos = 
        {
            T.pos.x - T.baseTextureOffset.x, 
            T.pos.y - T.baseTextureOffset.y
        };

        runExplosionAnimation = true;
        explosionAnimation(explPos, true);
    }
    PlaySound(explSoundP);
    
    Turret::validTurrets &= ~(1 << index);//turn off bit associated with correct turret
    winCond();
}

void Utils::PcollisionResolution(Player& P)
{
    PlaySound(explSoundP);

    P.lives--;
    if( P.lives < 0 ) gameState = GameState::PLAYERDIED;      
}

void Utils::explosionAnimation(Vector2 pos, bool newExpl)
{
    static int whichTextureRec;//will use this to keep track of how many explotion frames have gone by
    static int frameCounter;
    const  int explosionSpeed = 19;//explosion fps speed

    static Vector2 sPos;
    if(newExpl) sPos = pos;//check to see if re init of sPos should happen

    static Rectangle rec = 
    {//textureCoordinates              
        0.0f, 0.0f, 
        (float)explosionTexture.width/3, 
        (float)explosionTexture.height/3
    };

    frameCounter++;
    if(frameCounter >= (60/explosionSpeed))//after ten frames go by move texture coords
    {
        frameCounter = 0;
        whichTextureRec++;
        if(whichTextureRec > 8)
        {
            whichTextureRec = 0;
            runExplosionAnimation = false;
            rec = {0.0f, 0.0f, rec.width, rec.height};
            return;
        }
        rec.x = (whichTextureRec % 3) * rec.width;
        rec.y = (whichTextureRec / 3) * rec.height;
    }
    DrawTextureRec(explosionTexture, rec, sPos, WHITE);   
}

void Utils::debug(Invader* invaders, Turret* turrets, Player& player)
{
    for(int i = 0; i < Invader::max_invaders; ++i)
    {
        if(Invader::validInvaders & 1 << i)
        {
            DrawCircleLines(invaders[i].centerCircle.x, invaders[i].centerCircle.y, 6.0f, SKYBLUE);
            DrawCircleLines(invaders[i].leftCircle.x, invaders[i].leftCircle.y, 6.0f, SKYBLUE);
            DrawCircleLines(invaders[i].rightCircle.x, invaders[i].rightCircle.y, 6.0f, SKYBLUE);
        }
    }
    for(int i = 0; i < Turret::turretCount; i++)
    {
        DrawCircleLines(turrets[i].hitCircle.x, turrets[0].hitCircle.y, turrets[0].radius, WHITE);
    }
    DrawLine(player.pos.x, player.pos.y, player.pos.x, 0, WHITE);
}