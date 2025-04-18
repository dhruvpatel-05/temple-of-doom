//
//  Actor.h
//  Proj3
//
//  Created by Dhruv Patel on 5/12/24.
//

#ifndef Actor_h
#define Actor_h
#include <string>
#include "Weapon.h"
using namespace std;

class Actor
{
public:
    Actor(int hp, int a, int st, int d, int row, int col, Weapon w, string n);
    //getter methods
    int gethitPoints();
    int getArmor();
    int getStrength();
    int getDexterity();
    int getRow();
    int getCol();
    int getSleepTime();
    Weapon getWeapon();
    string getName();
    //setter methods
    virtual void sethitPoints(int hp);
    virtual void setArmor(int a);
    virtual void setStrength(int st);
    virtual void setDexterity(int d);
    void setRow(int r);
    void setCol(int c);
    void setSleepTime(int s);
    void setWeapon(Weapon w);

private:
    int hitPoints, armor, strength, dexterity, row, col, sleepTime;
    Weapon weapon;
    string name;
};

#endif /* Actor_h */
