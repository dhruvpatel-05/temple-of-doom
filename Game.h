// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED
#include "Level.h"

// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    void play();
    int getsmellDistanceGoblin();
private:
    int smellDistanceGoblin;
};


#endif // GAME_INCLUDED
