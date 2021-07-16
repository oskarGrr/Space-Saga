#include "gameStateStuff.h"


enum class GameState : unsigned int
{
    none, start, loose, win, gamePlay
};

GameState& getGameState()
{
    static GameState gameState = GameState::none;
    return gameState;
}



