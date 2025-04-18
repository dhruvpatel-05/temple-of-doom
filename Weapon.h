//
//  Weapon.h
//  Proj3
//
//  Created by Dhruv Patel on 5/20/24.
//
#ifndef Weapon_h
#define Weapon_h
#include "GameObject.h"
#include "utilities.h"
#include <string>
#include <cstdlib>
using namespace std;

class Weapon : public GameObject {
public:
    Weapon(const string& n, const string& a, int dB, int dA)
        : GameObject(n), actionString(a), dexterityBonus(dB), damageAmount(dA) {}
    
    //getter methods
    string getActionString() const { return actionString; }
    int getDexterityBonus() const { return dexterityBonus; }
    int getDamageAmount() const { return damageAmount; }

    virtual bool hit(int attackerDexterity, int defenderDexterity, int defenderArmorPoints, string& output, string defender, string attacker) {
        int attackerPoints = attackerDexterity + dexterityBonus;
        int defenderPoints = defenderDexterity + defenderArmorPoints;
        if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)){ //from spec; determines chance of hitting defender
            output = attacker +  " " + actionString + " the " + defender + " and hits";
            return true; //returns true if the defender has been hit
        }
        output = attacker + " " + actionString + " the " + defender + " and misses";
        return false; //returns false if the defender has not been hit
    }


private:
    string actionString;
    int dexterityBonus;
    int damageAmount;
};


//each of these classes are child classes of Weapon and use the weapon constructor in each of their constructors
class Mace : public Weapon {
public:
    Mace() : Weapon("mace", "swings mace at", 0, 2) {}
};

class ShortSword : public Weapon {
public:
    ShortSword() : Weapon("short sword", "slashes short sword at", 0, 2) {}
};

class LongSword : public Weapon {
public:
    LongSword() : Weapon("long sword", "swings long sword at", 2, 4) {}
};

class MagicAxe : public Weapon {
public:
    MagicAxe() : Weapon("magic axe", "chops magic axe at", 5, 5) {}
};

class MagicFangsOfSleep : public Weapon {
public:
    MagicFangsOfSleep() : Weapon("magic fangs of sleep", "strikes magic fangs of sleep at", 3, 2) {}

};

#endif /* Weapon_h */
