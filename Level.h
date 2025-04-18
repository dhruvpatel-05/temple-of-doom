//
//  Level.h
//  Proj3
//
//  Created by Dhruv Patel on 5/17/24.
//

#ifndef Level_h
#define Level_h
#include "Player.h"
#include <vector>
#include "Bogeymen.h"
#include "Snakewomen.h"
#include "Goblins.h"
#include "Dragons.h"
#include "Coord.h"
#include "GameObject.h"
using namespace std;

class Level{
  public:
    struct Room{
        int row, col, width, height;
    };
    Level();
    ~Level();
    void generateCorridors();
    bool overlap(Room r1, Room r2);
    void initGrid();
    void initActors();
    void updateGrid(char direction);
    void displayGrid();
    int mainCol();
    int mainRow();
    int getLevel();
    char getGrid(int row, int col);
    void setGrid(int row, int col, char cha);
    void displayInventory();
  private:
    Player main;
    int level;
    Room rList[8];;
    int numRooms;
    vector<Bogeymen> bogey;
    vector<Snakewomen> snaky;
    vector<Dragons> dragy;
    vector<Goblins> goby;
    vector<Coord> levelAdvance;
    vector<Coord> lastSymbol;
    vector<Coord> empty; //used to place actors on the map
    char grid[18][70];
    char lastVal;
    bool showGrid;
    Coord lastAdvancerPos;
    vector<GameObject*> inventory;
    int weaponIndex;
    int inventSize;
    string monsterAttack;
    string playerAttack;
    bool displayAttack;
    int numMonsters;
    int numGameObjects;
    string itemCollected;
    bool equipItem;
    bool win;
    char prevInput;
    Coord exitTracker;
    vector<Coord> goblinObjects;
    vector<Coord> bogeymanObjects;
    vector<Coord> snakewomanObjects;
    vector<Coord> dragonObjects;
};
//char grid[18][70];


#endif /* Level_h */
