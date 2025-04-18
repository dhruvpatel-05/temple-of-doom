//
//  Dragons.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/19/24.
//

#include <stdio.h>
#include "Dragons.h"
#include "utilities.h"
#include "Level.h"
#include <iostream>
using namespace std;


Dragons::Dragons() : Actor(randInt(20,25), 4, 4, 4, 0, 0, LongSword(), "Dragon")
{}

//no move/smell functions since the dragon does not move
