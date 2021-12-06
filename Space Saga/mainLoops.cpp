#include <raylib.h>
#include <iostream>

#include "Player.h"
#include "Invader.h"
#include "Bullet.h"
#include "Turret.h"
#include "utils.h"

static void endingLoop();

void mainLoop()
{
    InitWindow(Utils::screenWidth, Utils::screenHeight, "Space Saga");
    SetTargetFPS(60);
    //contains pointers to arrays of invaders/turrets/bullets and has utility methods
    Utils utils;
    
    while(!WindowShouldClose())
    {   
        if(Utils::gameState == GameState::GAMEWON || Utils::gameState == GameState::PLAYERDIED) endingLoop();                
        
        float deltaTime = GetFrameTime();
        float dynamicPlayerVelocity  =  deltaTime * Player::speedMultiplier;
        float dynamicBulletVelocity  =  deltaTime * Bullet::speedMultiplier;
        float dynamicInvaderVelocity =  deltaTime * Invader::speedMultiplier;
        
        BeginDrawing();
        ClearBackground({0, 0, 0, 255});

        Utils  ::drawUI(Utils::screenWidth, Utils::screenHeight, utils.player, utils.font);       
        Turret ::drawTurrets(utils.turrets, utils.player, utils.bullets);
        Invader::updateInvaders(Utils::screenWidth, Utils::screenHeight, dynamicInvaderVelocity, utils.invaders);
        Invader::drawInvaders(utils.invaders);
        Bullet ::manageBullets(utils.bullets, utils.invaders, utils.player, utils.turrets, deltaTime);
        Bullet ::drawBullets(utils.bullets, dynamicBulletVelocity, Utils::screenHeight, utils.player);
        Bullet ::checkForCollisions(utils.bullets, utils.invaders, utils.player, utils.turrets);

        utils.player.updatePlayer(Utils::screenWidth, dynamicPlayerVelocity, utils.bullets);
        utils.player.drawPlayer();

        EndDrawing();
    }
    CloseWindow();
}

void titleScreenLoop()
{   
    InitWindow(Utils::screenWidth, Utils::screenHeight, "Space Saga");
    SetTargetFPS(60);

    bool start = 0;//has the start button been pressed
    Texture2D titleScreen_backGround = LoadTexture("Resources/Textures/backGround1.png");
    Font f = LoadFont("");
    float time;

    while(!WindowShouldClose())
    {
        time = GetTime() * 1.7;
        BeginDrawing();
        ClearBackground({0, 0, 0, 255});

        DrawTexture(titleScreen_backGround, 0, 0, WHITE);
        DrawText("SPACE SAGA",0,0,50,WHITE);

        if((int)time & 1) {
             DrawTextEx(f,"press any key to start", 
                        {Utils::screenWidth * .3f -(MeasureText("press space to start", 20)*.5f), 10}, 
                        20.0f, 5.0f, WHITE);
        }
        
        if(GetKeyPressed())
        {
            CloseWindow();
            UnloadTexture(titleScreen_backGround);
            std::cout << "\n\n\n\n";
            return;
        }  
        
        EndDrawing();
    }
    UnloadTexture(titleScreen_backGround);
    CloseWindow();
    exit(EXIT_SUCCESS);
}

static void endingLoop()
{
    const int SW = 500;
    const int SH = 180;
    InitWindow(SW, SH, "boooo");
    SetTargetFPS(60);

    const float      btnWidth = SW * .8f;
    const float      btnHeight = 35.0f;
    const Rectangle  restart = {SW * .1f, 30.0f, btnWidth, btnHeight};   
    const Rectangle  quit = {SW * .1f, restart.y + 65.0f, btnWidth, btnHeight};
    const Vector2    restartBtnPos = {restart.x + btnWidth/2, restart.y + btnHeight/2};
    const Vector2    quitBtnPos = {quit.x + btnWidth/2, quit.y + btnHeight/2};
    const int        quitSize = MeasureText("QUIT", 16),  resrtartSize = MeasureText("RESTART", 16);
    const Color      btnColors[2] = {GRAY,LIGHTGRAY};

    int whichColorRestart = 1;//gets toggled depending on where mouse is
    int whichColorQuit = 1;//gets toggled depending on where mouse is

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({ 0, 0, 0, 255 });

        if(Utils::isMouseOver(restart)) whichColorRestart = 0;      
        else whichColorRestart = 1;

        if(Utils::isMouseOver(quit)) whichColorQuit = 0;    
        else whichColorQuit = 1;

        DrawRectangleRec(restart, btnColors[whichColorRestart]);
        DrawText("RESTART", restartBtnPos.x - resrtartSize/2, restartBtnPos.y - 9, 18, BLACK);
        DrawRectangleLinesEx(restart, 4, BLUE);

        DrawRectangleRec(quit, btnColors[whichColorQuit]);
        DrawText("QUIT", quitBtnPos.x - quitSize/2, quitBtnPos.y - 9, 18, BLACK);
        DrawRectangleLinesEx(quit, 4, BLUE);
        
        EndDrawing();
    }
    CloseWindow();
    exit(EXIT_SUCCESS);
}