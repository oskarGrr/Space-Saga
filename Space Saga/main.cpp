#include "Player.h"
#include "Bullet.h"
#include "Invader.h"
#include "Turret.h"
#include "declarationsAndGameloops.h"

#include <chrono>
#include <raylib.h>

#include <iostream>

//new invader texture new hit circles
//clean update bullets split into severl functions
//


int main()
{
    mainLoop();
}

void mainLoop()
{
    srand(time(NULL));
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Space Saga");
    SetTargetFPS(60);

    titleScreenLoop(screenWidth, screenHeight);

    Turret* turrets = Turret::initTurrets(screenWidth);

    Invader* invaders = Invader::initInvaders(screenHeight, screenWidth);
    bool direction = 0;

    Player player(screenWidth, screenHeight);
    Bullet::init_Bullet_Resources();                 //currently set to 16 max 
    Bullet* bullets = (Bullet*)malloc(sizeof(Bullet) * Bullet::max_bullets);//384 bytes//24 bytes per bullet
    // maybe will switch to alloca + placement new later

    std::chrono::steady_clock::time_point startP;
    std::chrono::steady_clock::time_point endP;
    std::chrono::duration<float> timeElapsedP;
    bool hasSpaceBeenPressedYet = false;

    std::chrono::steady_clock::time_point startI;
    std::chrono::steady_clock::time_point endI;
    std::chrono::duration<float> timeElapsedI = std::chrono::steady_clock::duration::zero();
    
    while(!WindowShouldClose())
    {
        double deltaTime = (double)GetFrameTime();
        double dynamicPlayerVelocity  =  deltaTime * Player::speedMultiplier;
        double dynamicBulletVelocity  =  deltaTime * Bullet::speedMultiplier;
        double dynamicInvaderVelocity =  deltaTime * Invader::speedMultiplier;
        //////////////////////////////////////////////////////////////////////
        BeginDrawing();
        ClearBackground({0, 0, 0, 255});
        //////////////////////////////////////////////////////////////////////

        
        player.updatePlayer(screenWidth, screenHeight, dynamicPlayerVelocity);
        Invader::updateInvaders(invaders, screenWidth, direction, dynamicInvaderVelocity);

        Invader::drawInvaders(invaders);
        Turret::drawTurrets(turrets, player);

        if (IsMouseButtonPressed(MouseButton::MOUSE_LEFT_BUTTON))
        {
            Bullet::manageBullets(Bullet::PLAYER, {player.x_pos, player.Y_pos}, 
                                                   bullets, timeElapsedP, startP, endP, hasSpaceBeenPressedYet, 
                                                   startI, endI, timeElapsedI, invaders);
        }
        Bullet::manageBullets(Bullet::INVADER, {player.x_pos, player.Y_pos}, 
                                                   bullets, timeElapsedP, startP, endP, hasSpaceBeenPressedYet, 
                                                   startI, endI, timeElapsedI, invaders);
        Bullet::drawBullets(bullets, dynamicBulletVelocity, screenHeight, player.playerWidth);
        Bullet::checkForCollisions(bullets, invaders, player);


        //////////////////////////////////////////////////////////////////////
        EndDrawing();
    }
    CloseWindow();
}

inline void titleScreenLoop(const int screenWidth, const int screenHeight)
{
    bool start = 0;//has the start button been pressed
    Texture2D titleScreen_backGround = LoadTexture("Resources/Textures/titleBackGround.png");
    Font f = LoadFont("Resources/Textures/vgaoem.fon");

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({0, 0, 0, 255});

        DrawTexture(titleScreen_backGround, 0, 0, WHITE);
        DrawText("SPACE SAGA",0,0,50,WHITE);
        DrawTextEx(f,"press space to start", 
                  {screenWidth * .3f -(MeasureText("press space to start", 20.0f)*.5f), 10}, 
                   20.0f, 5.0f, WHITE);

        if(IsKeyPressed(KeyboardKey::KEY_SPACE))return;

        //if(start)return;
        
        EndDrawing();
    }
    CloseWindow();
    exit(0);
}

void freeMem()
{

}