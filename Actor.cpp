//
//  Actor.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/12/24.
//

#include "Actor.h"
#include "utilities.h"
#include <iostream>
using namespace std;

//constructor
Actor::Actor(int hp, int a, int st, int d, int r, int c, Weapon w, string n) : hitPoints(hp), armor(a), strength(st), dexterity(d), sleepTime(0), row(r), col(c), weapon(w), name(n)
{}

//getter methods for actor attributes
int Actor::gethitPoints(){return hitPoints;}
int Actor::getArmor(){return armor;}
int Actor::getStrength(){return strength;}
int Actor::getDexterity(){return dexterity;}
int Actor::getRow(){return row;}
int Actor::getCol(){return col;}
int Actor::getSleepTime(){return sleepTime;}
Weapon Actor::getWeapon(){return weapon;}
string Actor::getName(){return name;}

//setter methods for actor attributes
void Actor::sethitPoints(int hp){hitPoints = hp;}
void Actor::setArmor(int a){armor = a;}
void Actor::setStrength(int st){strength = st;}
void Actor::setDexterity(int d){dexterity = d;}
void Actor::setRow(int r){row = r;}
void Actor::setCol(int c){col = c;}
void Actor::setSleepTime(int s){sleepTime = s;}
void Actor::setWeapon(Weapon w){weapon = w;}
