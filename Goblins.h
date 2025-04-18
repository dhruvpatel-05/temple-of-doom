//
//  Goblins.h
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#ifndef Goblins_h
#define Goblins_h
#include "Actor.h"
#include "Coord.h"
#include <vector>
using namespace std;
class Goblins : public Actor
{
public:
    Goblins();
    bool canSmellPlayer(int gobRow, int gobCol, int playerRow, int playerCol, char grid[18][70], int smellingDistance);
    void moveTowardPlayer(int playerRow, int playerCol, char grid[18][70]);
    void setPrevChar(char c);
private:
    char prevChar;
};


#endif /* Goblins_h */
