//
//  Snakewomen.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#include <stdio.h>
#include "Snakewomen.h"
#include "utilities.h"
#include "Level.h"
#include <iostream>
using namespace std;


Snakewomen::Snakewomen() : Actor(randInt(3,6), 3, 2, 3, 0, 0, MagicFangsOfSleep(), "Snakewoman"), prevChar(' ')
{}

bool Snakewomen::canSmellPlayer(int playerRow, int playerCol) {
    if(abs(playerRow - getRow()) + abs(playerCol - getCol()) <= 3){ //same as Bogeyman but can only smell 3 units instead of 5
        return true;
    }
    return false;
}

void Snakewomen::moveTowardPlayer(int playerRow, int playerCol, char grid[18][70]) {
    if (!canSmellPlayer(playerRow, playerCol)) {
        return;
    }

    int bestRow = getRow();
    int bestCol = getCol();

   if (abs(playerRow - (getRow() - 1)) < abs(playerRow - bestRow) && grid[getRow() - 1][getCol()] != '#') {
        bestRow = getRow() - 1;
    }
    else if (abs(playerRow - (getRow() + 1)) < abs(playerRow - bestRow) && grid[getRow() + 1][getCol()] != '#') {
        bestRow = getRow() + 1;
    }
    else if (abs(playerCol - (getCol() - 1)) < abs(playerCol - bestCol) && grid[getRow()][getCol() - 1] != '#') {
        bestCol = getCol() - 1;
    }
    else if (abs(playerCol - (getCol() + 1)) < abs(playerCol - bestCol) && grid[getRow()][getCol() + 1] != '#') {
        bestCol = getCol() + 1;
    }

    if ((bestRow != getRow() || bestCol != getCol()) && grid[bestRow][bestCol] != '#' && grid[bestRow][bestCol] != '@'  && grid[bestRow][bestCol] != 'B'  && grid[bestRow][bestCol] != 'G'  && grid[bestRow][bestCol] != 'D'  && grid[bestRow][bestCol] != 'S') {
        grid[getRow()][getCol()] =  prevChar;
        setRow(bestRow);
        setCol(bestCol);
        prevChar = grid[getRow()][getCol()];

        grid[getRow()][getCol()] =  'S';
    }
    //same implementation as the bogeyman moveTowardPlayer function
}

void Snakewomen::setPrevChar(char c){
    prevChar = c;
}
