//
//  Bogeymen.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#include <stdio.h>
#include "Bogeymen.h"
#include "utilities.h"
#include "Level.h"
#include "Game.h"
#include <iostream>
using namespace std;


Bogeymen::Bogeymen() : Actor(randInt(5,10), randInt(2,3), randInt(2,3), 2, 0, 0, ShortSword(), "Bogeyman"), prevChar(' ')
{}


bool Bogeymen::canSmellPlayer(int playerRow, int playerCol) {
    if(abs(playerRow - getRow()) + abs(playerCol - getCol()) <= 5){
        return true; //bogeyman can smell if within 5 units from player
    }
    return false;
}

void Bogeymen::moveTowardPlayer(int playerRow, int playerCol, char grid[18][70]) {
    if (!canSmellPlayer(playerRow, playerCol)) {
        return;
    } //exits function if player can not be smelled

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
    //has bogeyman move in 1 of 4 directions in order to move closer to the player

    if ((bestRow != getRow() || bestCol != getCol()) && grid[bestRow][bestCol] != '#' && grid[bestRow][bestCol] != '@'  && grid[bestRow][bestCol] != 'B'  && grid[bestRow][bestCol] != 'G'  && grid[bestRow][bestCol] != 'D'  && grid[bestRow][bestCol] != 'S') {
        grid[getRow()][getCol()] =  prevChar;
        setRow(bestRow);
        setCol(bestCol);
        prevChar = grid[getRow()][getCol()];
        grid[getRow()][getCol()] =  'B';
        //updates the position of the bogeyman and updates the former position of the Bogeyman
    }
}

void Bogeymen::setPrevChar(char c){
    prevChar = c;
}
