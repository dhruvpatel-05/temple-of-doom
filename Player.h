//
//  Player.h
//  Proj3
//
//  Created by Dhruv Patel on 5/13/24.
//

#ifndef Player_h
#define Player_h
#include "Actor.h"
#include "Snakewomen.h"
#include <vector>
using namespace std;
class Player : public Actor
{
public:
    Player();
    void displayStats(int level);
    void move(char direction, char (&grid)[18][70], vector<Snakewomen>& snaky, int& newRow, int& newCol);
    //getter method
    int getmaxhitPoints();
    //setter methods
    void sethitPoints(int hp);
    void setArmor(int a);
    void setDexterity(int d);
    void setStrength(int st);
    void setmaxhitPoints(int hp);
    void setPrevChar(char c);
private:
    int maxHitPoints;
    char prevChar;

};

#endif /* Player_h */
