//
//  Goblins.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#include <stdio.h>
#include "Goblins.h"
#include "utilities.h"
#include "Level.h"
#include "Coord.h"
#include <queue>
class Game;
#include <iostream>
using namespace std;


Goblins::Goblins() : Actor(randInt(15,20), 1, 3, 1, 0, 0, ShortSword(), "Goblin"), prevChar(' ')
{}

bool Goblins::canSmellPlayer(int gobRow, int gobCol, int playerRow, int playerCol, char grid[18][70], int smellingDistance) {
    // Base cases
    if (gobRow == playerRow && gobCol == playerCol) {
        return true;
    }
    if (smellingDistance == 0) {
        return false;
    }

    // Mark current position as visited
    grid[gobRow][gobCol] = '%';

    // Up
    if (gobRow > 0 && grid[gobRow - 1][gobCol] != '#' && grid[gobRow - 1][gobCol] != '%' && grid[gobRow - 1][gobCol] != 'B' && grid[gobRow - 1][gobCol] != 'D' && grid[gobRow - 1][gobCol] != 'G' && grid[gobRow - 1][gobCol] != 'S' && canSmellPlayer(gobRow - 1, gobCol, playerRow, playerCol, grid, smellingDistance - 1)) {
        return true;
    }
    // Down
    else if (gobRow < 17 && grid[gobRow + 1][gobCol] != '#' && grid[gobRow + 1][gobCol] != '%' && grid[gobRow + 1][gobCol] != 'S' && grid[gobRow + 1][gobCol] != 'G' && grid[gobRow + 1][gobCol] != 'D' && grid[gobRow + 1][gobCol] != 'B' && canSmellPlayer(gobRow + 1, gobCol, playerRow, playerCol, grid, smellingDistance - 1)) {
        return true;
    }
    // Left
    else if (gobCol > 0 && grid[gobRow][gobCol - 1] != '#' && grid[gobRow][gobCol - 1] != '%' && grid[gobRow][gobCol - 1] != 'B' && grid[gobRow][gobCol - 1] != 'D' && grid[gobRow][gobCol - 1] != 'G' && grid[gobRow][gobCol - 1] != 'S' && canSmellPlayer(gobRow, gobCol - 1, playerRow, playerCol, grid, smellingDistance - 1)) {
        return true;
    }
    // Right
    else if (gobCol < 69 && grid[gobRow][gobCol + 1] != '#' && grid[gobRow][gobCol + 1] != '%' && grid[gobRow][gobCol + 1] != 'G'  && grid[gobRow][gobCol + 1] != 'S'  && grid[gobRow][gobCol + 1] != 'D'  && grid[gobRow][gobCol + 1] != 'B' && canSmellPlayer(gobRow, gobCol + 1, playerRow, playerCol, grid, smellingDistance - 1)) {
        return true;
    }
    //recursive calls bring the function paramters closer to the base cases

    return false;
}

void Goblins::moveTowardPlayer(int playerRow, int playerCol, char grid[18][70]) {
    char maze[18][70];
    for(int i=0; i < 18; i ++){
        for(int j=0; j < 70; j ++){
            maze[i][j] = grid[i][j];
        }
    } //duplicates grid into maze, so grid isn't modified by canSmellPlayer function
    if (!canSmellPlayer(getRow(), getCol(), playerRow, playerCol, maze, 60)) {
        return;
    } //exits if the goblin can not smell the player

    int bestRow = getRow();
    int bestCol = getCol();
    
    //up
    if (abs(playerRow - (getRow() - 1)) < abs(playerRow - bestRow) && grid[getRow() - 1][getCol()] != '#'  && grid[bestRow-1][bestCol] != 'B'  && grid[bestRow-1][bestCol] != 'G'  && grid[bestRow-1][bestCol] != 'D'  && grid[bestRow-1][bestCol] != 'S') {
         bestRow = getRow() - 1;
     }
    //down
    else if (abs(playerRow - (getRow() + 1)) < abs(playerRow - bestRow) && grid[getRow() + 1][getCol()] != '#'  && grid[bestRow+1][bestCol] != 'B'  && grid[bestRow+1][bestCol] != 'G'  && grid[bestRow+1][bestCol] != 'D'  && grid[bestRow+1][bestCol] != 'S') {
         bestRow = getRow() + 1;
     }
    //left
    else if (abs(playerCol - (getCol() - 1)) < abs(playerCol - bestCol) && grid[getRow()][getCol() - 1] != '#'  && grid[bestRow][bestCol-1] != 'B'  && grid[bestRow][bestCol-1] != 'G'  && grid[bestRow][bestCol-1] != 'D'  && grid[bestRow][bestCol-1] != 'S') {
         bestCol = getCol() - 1;
     }
    //right
    else if (abs(playerCol - (getCol() + 1)) < abs(playerCol - bestCol) && grid[getRow()][getCol() + 1] != '#'  && grid[bestRow][bestCol+1] != 'B'  && grid[bestRow][bestCol+1] != 'G'  && grid[bestRow][bestCol+1] != 'D'  && grid[bestRow][bestCol+1] != 'S') {
         bestCol = getCol() + 1;
     }
    
    //if any of the above if statements executed....
     if ((bestRow != getRow() || bestCol != getCol()) && grid[bestRow][bestCol] != '#' && grid[bestRow][bestCol] != '@'  && grid[bestRow][bestCol] != 'B'  && grid[bestRow][bestCol] != 'G'  && grid[bestRow][bestCol] != 'D'  && grid[bestRow][bestCol] != 'S') {
         grid[getRow()][getCol()] =  prevChar;
         setRow(bestRow);
         setCol(bestCol);
         prevChar = grid[getRow()][getCol()];
         grid[getRow()][getCol()] =  'G'; //updates the position of the goblin so that it moves
     }
 }

void Goblins::setPrevChar(char c){
    prevChar = c;
}
