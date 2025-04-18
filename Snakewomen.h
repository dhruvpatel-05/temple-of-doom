//
//  Snakewomen.h
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#ifndef Snakewomen_h
#define Snakewomen_h
#include "Actor.h"

class Snakewomen : public Actor
{
public:
    Snakewomen();
    bool canSmellPlayer(int playerRow, int playerCol);
    void moveTowardPlayer(int playerRow, int playerCol, char grid[18][70]);
    void setPrevChar(char c);
private:
    char prevChar;
    
};

#endif /* Snakewomen_h */
