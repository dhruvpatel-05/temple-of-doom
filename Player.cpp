//
//  Player.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/13/24.
//

#include "Player.h"
#include "utilities.h"
#include "Level.h"
#include <iostream>
#include <vector>
using namespace std;


Player::Player() : Actor(20, 2, 2, 2, 0, 0, LongSword(), "Player"), maxHitPoints(20), prevChar(' ')
{}

void Player::sethitPoints(int hp){
    if(hp > maxHitPoints){
        Actor::sethitPoints(maxHitPoints);
    } //ensures hitPoints does not excede maxHitPoints
    else{
        Actor::sethitPoints(hp);
    }
}//updates hitPoints

int Player::getmaxhitPoints(){
    return maxHitPoints; //returns maxHitPoints
}

void Player::setmaxhitPoints(int hp){
    if(hp > 99){
        maxHitPoints = 99;
    } //maxHitPoints never goes above 99
    else{
        maxHitPoints = hp;
    }
} //changes maxHitPoints

void Player::setArmor(int a){
    if(a > 99){
        Actor::setArmor(99);
    } //armor never goes above 99
    else{
        Actor::setArmor(a);
    }
} //changes armor

void Player::setStrength(int st){
    if(st > 99){
        Actor::setStrength(99);
    } //strength never goes above 99
    else{
        Actor::setStrength(st);
    }
} //changes strength

void Player::setDexterity(int d){
    if(d > 99){
        Actor::setDexterity(99);
    } //dexterity never goes above 99
    else{
        Actor::setDexterity(d);
    }
} //changes dexterity




void Player::displayStats(int level){
    cout << "Level: "  << level << ", Hit points: " << gethitPoints() << ", Armor: " << getArmor() << ", Strength: " << getStrength() << ", Dexterity: " << getDexterity();
} //displays player stats underneath the grid

void move(char direction, char (&grid)[18][70], vector<Snakewomen>& snaky, int& newRow, int& newCol) {

    if (direction == ARROW_UP && newRow > 0) {
        if (grid[newRow - 1][newCol] != '#') {
            if (grid[newRow - 1][newCol] == 'S') {
                // Handle collision with Snakewoman
                for (int i = 0; i < snaky.size(); i++) {
                    if (newRow - 1 == snaky[i].getRow() && newCol == snaky[i].getCol()) {
                        snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0, 4));
                        if (snaky[i].gethitPoints() <= 0) {
                            grid[newRow - 1][newCol] = ' ';
                        }
                    }
                }
            } else {
                newRow--;
            }
        }
    } else if (direction == ARROW_DOWN && newRow < 17) {
        if (grid[newRow + 1][newCol] != '#') {
            if (grid[newRow + 1][newCol] == 'S') {
                // Handle collision with Snakewoman
                for (int i = 0; i < snaky.size(); i++) {
                    if (newRow + 1 == snaky[i].getRow() && newCol == snaky[i].getCol()) {
                        snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0, 4));
                        if (snaky[i].gethitPoints() <= 0) {
                            grid[newRow + 1][newCol] = ' ';
                        }
                    }
                }
            } else {
                newRow++;
            }
        }
    } else if (direction == ARROW_LEFT && newCol > 0) {
        if (grid[newRow][newCol - 1] != '#') {
            if (grid[newRow][newCol - 1] == 'S') {
                // Handle collision with Snakewoman
                for (int i = 0; i < snaky.size(); i++) {
                    if (newRow == snaky[i].getRow() && newCol - 1 == snaky[i].getCol()) {
                        snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0, 4));
                        if (snaky[i].gethitPoints() <= 0) {
                            grid[newRow][newCol - 1] = ' ';
                        }
                    }
                }
            } else {
                newCol--;
            }
        }
    } else if (direction == ARROW_RIGHT && newCol < 69) {
        if (grid[newRow][newCol + 1] != '#') {
            if (grid[newRow][newCol + 1] == 'S') {
                // Handle collision with Snakewoman
                for (int i = 0; i < snaky.size(); i++) {
                    if (newRow == snaky[i].getRow() && newCol + 1 == snaky[i].getCol()) {
                        snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0, 4));
                        if (snaky[i].gethitPoints() <= 0) {
                            grid[newRow][newCol + 1] = ' ';
                        }
                    }
                }
            } else {
                newCol++;
            }
        }
    }
}

void Player::setPrevChar(char c){
    prevChar = c;
}
