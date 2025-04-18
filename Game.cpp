// Game.cpp

#include "Game.h"
#include "Level.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance): smellDistanceGoblin(goblinSmellDistance)
{}

int Game::getsmellDistanceGoblin(){
    return smellDistanceGoblin;
}

void Game::play()
{
    Level initial = Level();
    char ch;
    while (true) {
        ch = getCharacter(); // Wait for a keypress
        if (ch == 'q') {
            break;
        }
        initial.updateGrid(ch); // Update the grid based on the character input
    }
}

// You will presumably add to this project other .h/.cpp files for the various
// classes of objects you need to play the game:  player, monsters, weapons,
// etc.  You might have a separate .h/.cpp pair for each class (e.g., Player.h,
// Bogeyman.h, etc.), or you might put the class declarations for all actors in
// Actor.h, all game objects in GameObject.h, etc.
