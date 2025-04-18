//
//  Scroll.h
//  Proj3
//
//  Created by Dhruv Patel on 5/20/24.
//

#ifndef Scroll_h
#define Scroll_h

#include "GameObject.h"
#include "Player.h"
#include "Coord.h"
#include <string>
class Level;
using namespace std;

class Scroll : public GameObject {
public:
    Scroll(const std::string& name) : GameObject(name) {}
    virtual void applyEffect(Player& player) = 0;
};

class ScrollOfTeleportation : public Scroll {
public:
    ScrollOfTeleportation() : Scroll("a scroll called scroll of teleportation") {}
    void applyEffect(Player& player){}
    void applyTP(Player& player, char (&grid)[18][70]) {
        // Teleport the player to a random location
        vector<Coord> empty;
        for(int i=0; i < 18; i ++){
            for(int j=0; j < 70; j ++){
                if(grid[i][j] == ' '){
                    empty.push_back(Coord(i,j)); //adds every empty spot in the grid to the empty vector
                }
            }
        }
        int randIndex = randInt(0, empty.size() - 1); //generates a random index from empty
        Coord temp = empty[randIndex];
        grid[player.getRow()][player.getCol()] = ' ';
        grid[temp.r()][temp.c()] = '@'; //moves the player to a random empty spot on the grid
        player.setRow(temp.r());
        player.setCol(temp.c());
        empty.erase(empty.begin() + randIndex);
        
    }
};

//applyEffect for each of the scroll classes (child class of Scroll class) is used to implement scroll functionality
class ScrollOfImproveArmor : public Scroll {
public:
    ScrollOfImproveArmor() : Scroll("a scroll called scroll of improve armor") {}

    void applyEffect(Player& player) override {
        player.setArmor(player.getArmor() + randInt(1,3)); //increases armor when called
    }
};

class ScrollOfRaiseStrength : public Scroll {
public:
    ScrollOfRaiseStrength() : Scroll("a scroll called scroll of raise strength") {}

    void applyEffect(Player& player) override {
        player.setStrength(player.getStrength() + randInt(1,3)); //increases strength when called
    }
};

class ScrollOfEnhanceHealth : public Scroll {
public:
    ScrollOfEnhanceHealth() : Scroll("a scroll called scroll of enhance health") {}

    void applyEffect(Player& player) override {
        player.setmaxhitPoints(player.getmaxhitPoints() + randInt(3,8)); //increases max hit points when called
    }
};

class ScrollOfEnhanceDexterity : public Scroll {
public:
    ScrollOfEnhanceDexterity() : Scroll("A scroll called scroll of enhance dexterity") {}

    void applyEffect(Player& player) override {
        player.setDexterity(player.getDexterity() + 1); //increases dexterity when called
    }
};

#endif /* Scroll_h */

