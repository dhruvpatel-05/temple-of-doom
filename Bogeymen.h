//
//  Bogeymen.h
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#ifndef Bogeymen_h
#define Bogeymen_h
#include "Actor.h"
class Bogeymen : public Actor
{
public:
    Bogeymen();
    bool canSmellPlayer(int playerRow, int playerCol);
    void moveTowardPlayer(int playerRow, int playerCol, char grid[18][70]);
    void setPrevChar(char c);
private:
    char prevChar;
};

#endif /* Bogeymen_h */
