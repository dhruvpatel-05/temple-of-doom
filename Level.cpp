//
//  Level.cpp
//  Proj3
//
//  Created by Dhruv Patel on 5/17/24.
//

#include <stdio.h>
#include <iostream>
#include "Level.h"
#include "Player.h"
#include "Coord.h"
#include "utilities.h"
#include "GameObject.h"
#include "Weapon.h"
#include "Scroll.h"
#include <string>
#include <cmath>
#include <vector>
using namespace std;

//constructor
Level::Level() : lastAdvancerPos(0,0), exitTracker(0, 0){
    level = 0;
    lastVal = ' ';
    showGrid = false;
    inventSize = 1;
    inventory.push_back(new ShortSword());
    weaponIndex = 0;
    playerAttack = "";
    monsterAttack = "";
    displayAttack = false;
    numGameObjects = 0;
    numMonsters = 0;
    itemCollected =  "";
    prevInput = ' ';
    win = false;
    equipItem = false;
    initGrid();
    displayGrid();
}
Level::~Level(){
    for(int i = 0; i < inventSize; i++){
        delete inventory[i];
    } //deletes the game object points that are stored in the inventory
}

//finds coordinates for different actors and modifies the grid to display them
void Level::initActors(){
    bogey.clear();
    snaky.clear();
    goby.clear();
    dragy.clear();
    levelAdvance.clear();
    //clears all vectors
    
    for(int i=0; i < 18; i ++){
        for(int j=0; j < 70; j ++){
            if(grid[i][j] == ' '){
                empty.push_back(Coord(i,j)); //adds every empty spot in the grid to the empty vector
            }
        }
    }
    int randIndex = randInt(0, empty.size() - 1); //generates a random index from empty
    Coord temp = empty[randIndex];
    grid[temp.r()][temp.c()] = '@'; //random index is used to place @ (player) on the grid
    main.setRow(temp.r());
    main.setCol(temp.c());
    empty.erase(empty.begin() + randIndex);
    numMonsters = randInt(2,5*(level+1)+1); //calculates num of monsters on each level
    int tempMonstersNum = randInt(0,numMonsters); //decides how many snakewomen
    if(level > 1){ //Bogeymen
        for (int i = 0; i < tempMonstersNum; i++) {
            randIndex = randInt(0,empty.size() - 1);
            Coord temp = empty[randIndex];
            bogey.push_back(Bogeymen());
            bogey[i].setRow(temp.r());
            bogey[i].setCol(temp.c());
            bogey[i].setPrevChar(grid[temp.r()][temp.c()]);
            grid[temp.r()][temp.c()] = 'B'; //random index to place b (bogeyman) on the grid
            empty.erase(empty.begin() + randIndex);
        }
        numMonsters -= tempMonstersNum;
    }
    tempMonstersNum = randInt(0,numMonsters);
    
    if(level > 2){ //Dragons
        for (int i = 0; i < tempMonstersNum; i++) {
            randIndex = randInt(0, empty.size() - 1);
            temp = empty[randIndex];
            grid[temp.r()][temp.c()] = 'D'; //random index to place d (dragon) on the grid
            empty.erase(empty.begin() + randIndex);
            dragy.push_back(Dragons());
            dragy[i].setRow(temp.r());
            dragy[i].setCol(temp.c());
        }
        numMonsters -= tempMonstersNum;
    }
    tempMonstersNum = randInt(0,numMonsters); //decides how many dragons
    
    for (int i = 0; i < tempMonstersNum; i++) { //Snakewomen
        randIndex = randInt(0, empty.size() - 1);
        Coord temp = empty[randIndex];
        snaky.push_back(Snakewomen());
        snaky[i].setRow(temp.r());
        snaky[i].setCol(temp.c());
        snaky[i].setPrevChar(grid[temp.r()][temp.c()]);
        grid[temp.r()][temp.c()] = 'S'; //random index to place S (snakewoman) on the grid
        empty.erase(empty.begin() + randIndex);
    }
    numMonsters -= tempMonstersNum;
    for (int i = 0; i < numMonsters; i++) { //Goblins
        randIndex = randInt(0, empty.size() - 1);
        Coord temp = empty[randIndex];
        goby.push_back(Goblins());
        goby[i].setRow(temp.r());
        goby[i].setCol(temp.c());
        goby[i].setPrevChar(grid[temp.r()][temp.c()]);
        grid[temp.r()][temp.c()] = 'G'; //random index to place G (goblin) on the grid
        empty.erase(empty.begin() + randIndex);
    }
    
    
    
    if(level < 4){ //adds '>' if level < 4
        randIndex = randInt(0, empty.size() - 1);
        temp = empty[randIndex];
        grid[temp.r()][temp.c()] = '>'; //places this at a random empty spot in the grid
        exitTracker.setr(temp.r());
        exitTracker.setc(temp.c());
        empty.erase(empty.begin() + randIndex);
        levelAdvance.push_back(Coord(temp.r(), temp.c()));
    } //when level is 4 the golden idol is placed instead
    if(level == 4){
        randIndex = randInt(0, empty.size() - 1);
        temp = empty[randIndex];
        grid[temp.r()][temp.c()] = '&';  //places this at a random empty spot in the grid
        empty.erase(empty.begin() + randIndex);
    }
    numGameObjects = randInt(2,3); //used to find number of weapons/scrolls on the map
    int numWeapons = randInt(0, numGameObjects);
    for(int i = 0; i < numWeapons; i ++){
        randIndex = randInt(0, empty.size() - 1);
        temp = empty[randIndex];
        grid[temp.r()][temp.c()] = ')'; //adds weapons to a random empty position on the grid
        empty.erase(empty.begin() + randIndex);
    }
    for(int i = 0; i < numGameObjects - numWeapons; i ++){
        randIndex = randInt(0, empty.size() - 1);
        temp = empty[randIndex];
        grid[temp.r()][temp.c()] = '?';  //adds scrolls to a random empty position on the grid
        empty.erase(empty.begin() + randIndex);
    }
}

void Level::generateCorridors() {
    // Connect each room to the next room
    for (int i = 0; i < numRooms - 1; i++) {
        // Calculate the center coordinates of the current room and the next room
        int center1_x = round(rList[i].col + rList[i].width / 2);
        int center1_y = round(rList[i].row + rList[i].height / 2);
        int center2_x = round(rList[i + 1].col + rList[i + 1].width / 2);
        int center2_y = round(rList[i + 1].row + rList[i + 1].height / 2);
        center1_x = max(0, min(center1_x, 69));
        center1_y = max(0, min(center1_y, 17));
        center2_x = max(0, min(center2_x, 69));
        center2_y = max(0, min(center2_y, 17));
        // Draw a horizontal corridor from center1_x to center2_x
        for (int x = min(center1_x, center2_x); x <= max(center1_x, center2_x); x++) {
            if (center1_y > 0 && center1_y < 18) {
                grid[center1_y][x] = ' ';
            }
        }
        
        // Draw a vertical corridor from center1_y to center2_y
        for (int y = min(center1_y, center2_y); y <= max(center1_y, center2_y); y++) {
            if (center2_x > 0 && center2_x < 70) {
                grid[y][center2_x] = ' ';
            }
        }
    }
}


bool Level::overlap(Room r1, Room r2) {
    return !(r1.col + r1.width +2 <= r2.col ||
             r2.col + r2.width +2<= r1.col ||
             r1.row + r1.height +2<= r2.row ||
             r2.row + r2.height +2<= r1.row);
} //checks overlap between 2 room paramters. (+2 allows for some space between rooms)

void Level::initGrid(){
    for(int i=0; i < 18; i ++){
        for(int j=0; j < 70; j ++){
            grid[i][j] = '#'; //creates grid full of # to start with
        }
    }
    numRooms = randInt(4, 6); //used to decide the number of rooms in the grid
    for(int d = 0; d < numRooms; d ++){
        //randomizes room height and width
        rList[d].row = randInt(3,15);
        rList[d].col = randInt(5,65);
        rList[d].width = randInt(7, 10);
        rList[d].height = randInt(4,8);
        while(rList[d].col + rList[d].width > 68){
            rList[d].col--; //reduces column to prevent room from passing edge of the grid
        }
        while(rList[d].row + rList[d].height > 17){
            rList[d].row--; //reduces row to prevent room from passing edge of the grid
        }
        
        bool isOverlap = false;
        for (int i = 0; i < d; i++) {
            if (overlap(rList[d], rList[i])) {
                isOverlap = true;
                break;
            }
        }

        // If there's an overlap, generate a new room
        if (isOverlap) {
            d--; // d-- to generate a new room in the next iteration
            continue;
        }

        for(int i = rList[d].col; i < rList[d].col + rList[d].width; i ++){
            for(int j = rList[d].row; j < rList[d].row + rList[d].height; j ++){
                grid[j][i] = ' '; //creates the room by iterating through rList and replacing values with ' '
            }
        }
    }
    generateCorridors();
    initActors();
}


void Level::updateGrid(char direction) {
    if (!showGrid && (direction == 'w' || direction == 'r' || direction == 'i')) {
        clearScreen();
        displayInventory(); //displays inventory when w, r, or i are pressed by the user
        showGrid = true;
        equipItem = true;
        prevInput = direction;
    }
    else if(equipItem){
        equipItem = false;
        showGrid = false;
        char alp[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        for(int i = 0; i < inventSize; i++){
            if(alp[i] == direction){
                Weapon* weap = dynamic_cast<Weapon*>(inventory[i]); //dyanmic cast to figure out if game object is weapon or scroll
                if(weap != nullptr && prevInput == 'w'){
                    itemCollected = "You are wielding " + inventory[i]->getName(); //wielding statement
                    LongSword* ls = dynamic_cast<LongSword*>(inventory[i]); //more dynamic cast to distinguish different weapons
                    if (ls != nullptr) {
                        main.setWeapon(LongSword());
                    }
                    Mace* m = dynamic_cast<Mace*>(inventory[i]); //more dynamic cast to distinguish different weapons
                    if (m != nullptr) {
                        main.setWeapon(Mace());
                    }
                    MagicAxe* ma = dynamic_cast<MagicAxe*>(inventory[i]); //more dynamic cast to distinguish different weapons
                    if (ma != nullptr) {
                        main.setWeapon(MagicAxe());
                    }
                    MagicFangsOfSleep* mfos = dynamic_cast<MagicFangsOfSleep*>(inventory[i]); //more dynamic cast to distinguish different weapons
                    if (mfos != nullptr) {
                        main.setWeapon(MagicFangsOfSleep());
                    }
                    ShortSword* ss = dynamic_cast<ShortSword*>(inventory[i]); //more dynamic cast to distinguish different weapons
                    if (ss != nullptr) {
                        main.setWeapon(ShortSword());
                    }
                }
                else if(weap == nullptr && prevInput == 'w'){
                    itemCollected = "You can't read a " + inventory[i]->getName(); //player can't read a weapon
                }
                else if(weap != nullptr && prevInput == 'r'){
                    itemCollected = "You can't wield a " + inventory[i]->getName(); //player can't read a scroll
                }
                else if(prevInput == 'r' && weap == nullptr){
                    itemCollected = "You read the scroll called " + inventory[i]->getName(); //reading statement
                    ScrollOfTeleportation* st = dynamic_cast<ScrollOfTeleportation*>(inventory[i]); //more dynamic cast to distinguish different scrolls
                    if (st != nullptr) {
                        itemCollected += "\nYou feel your body wrenched in space and time.";
                        ScrollOfTeleportation().applyTP(main, grid);
                    }
                    ScrollOfImproveArmor* sia = dynamic_cast<ScrollOfImproveArmor*>(inventory[i]); //more dynamic cast to distinguish different scrolls
                    if (sia != nullptr) {
                        itemCollected += "\nYour armor glows blue.";
                        ScrollOfImproveArmor().applyEffect(main);
                    }
                    ScrollOfEnhanceHealth* seh = dynamic_cast<ScrollOfEnhanceHealth*>(inventory[i]); //more dynamic cast to distinguish different scrolls
                    if (seh != nullptr) {
                        itemCollected += "\nYou feel your heart beating stronger";
                        ScrollOfEnhanceHealth().applyEffect(main);
                    }
                    ScrollOfRaiseStrength* srs = dynamic_cast<ScrollOfRaiseStrength*>(inventory[i]); //more dynamic cast to distinguish different scrolls
                    if (srs != nullptr) {
                        itemCollected += "\nYour muscles bulge.";
                        ScrollOfRaiseStrength().applyEffect(main);
                    }
                    ScrollOfEnhanceDexterity* sed = dynamic_cast<ScrollOfEnhanceDexterity*>(inventory[i]); //more dynamic cast to distinguish different scrolls
                    if (sed != nullptr) {
                        itemCollected += "\nYou feel like less of a klutz.";
                        ScrollOfEnhanceDexterity().applyEffect(main);
                    }
                    delete inventory[i]; //deletes the pointer to the game object(scroll) after it is used
                    inventory.erase(inventory.begin() + i);
                    inventSize--;
                }
            }
        }
        clearScreen();
        displayGrid();
        if(itemCollected != ""){
            cout << itemCollected << endl;
        } //only prints the item used if it is set to something in the above code
    }
    else if(main.getSleepTime() > 0){ //when player is asleep they can't move, only the monsters move
        main.setSleepTime(main.getSleepTime() - 1); //decreases sleep time by 1 each turn
            //used for when the snakewoman is above the player
            if(grid[main.getRow() - 1][main.getCol()] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(main.getRow() - 1 == snaky[i].getRow() && main.getCol() == snaky[i].getCol()){
                                displayAttack = true;
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){ //1/5 chance of causing the player to sleep when hit
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){ //when already asleep, sleep is updated to highest of two values
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }
                                }
                            }
                    }
            }
        //used for when the bogeyman is above the player
            else if(grid[main.getRow()-1][main.getCol()] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(main.getRow() - 1== bogey[i].getRow() && main.getCol() == bogey[i].getCol()){
                                displayAttack = true;
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
        //used for when the dragon is above the player
            else if(grid[main.getRow()-1][main.getCol()] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(main.getRow() - 1 == dragy[i].getRow() && main.getCol() == dragy[i].getCol()){
                                displayAttack = true;
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].gethitPoints() <= 0){ //always spawns a scroll when the dragon dies
                                    grid[main.getRow()-1][main.getCol()] = '?';
                                    dragy.erase(dragy.begin() + i);
                                    monsterAttack = "";
                                }
                                
                            }
                    }
            }
        //used for when the goblin is above the player
            else if(grid[main.getRow() - 1][main.getCol()] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(main.getRow() - 1 == goby[i].getRow() && main.getCol() == goby[i].getCol()){
                                displayAttack = true;
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
        //rest of the else if statements below are parallel to the four if statements. They just use different directions
            else if(grid[main.getRow() + 1][main.getCol()] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(main.getRow() + 1 == snaky[i].getRow() && main.getCol() == snaky[i].getCol()){
                                displayAttack = true;
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }
                                }
                            }
                    }
            }
            else if(grid[main.getRow()+1][main.getCol()] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(main.getRow() + 1== bogey[i].getRow() && main.getCol() == bogey[i].getCol()){
                                displayAttack = true;
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else if(grid[main.getRow()+1][main.getCol()] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(main.getRow() + 1 == dragy[i].getRow() && main.getCol() == dragy[i].getCol()){
                                displayAttack = true;
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else if(grid[main.getRow() + 1][main.getCol()] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(main.getRow() + 1 == goby[i].getRow() && main.getCol() == goby[i].getCol()){
                                displayAttack = true;
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()+1] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(main.getRow() == snaky[i].getRow() && main.getCol()+ 1 == snaky[i].getCol()){
                                displayAttack = true;
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }

                                }
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()+ 1] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(main.getRow()== bogey[i].getRow() && main.getCol()+ 1 == bogey[i].getCol()){
                                displayAttack = true;
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()+ 1] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(main.getRow() == dragy[i].getRow() && main.getCol()+ 1 == dragy[i].getCol()){
                                displayAttack = true;
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                                
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()+ 1] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(main.getRow() == goby[i].getRow() && main.getCol()+ 1 == goby[i].getCol()){
                                displayAttack = true;
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()-1] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(main.getRow() == snaky[i].getRow() && main.getCol()- 1 == snaky[i].getCol()){
                                displayAttack = true;
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }

                                }
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()- 1] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(main.getRow()== bogey[i].getRow() && main.getCol()- 1 == bogey[i].getCol()){
                                displayAttack = true;
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()- 1] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(main.getRow() == dragy[i].getRow() && main.getCol()- 1 == dragy[i].getCol()){
                                displayAttack = true;
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                                
                            }
                    }
            }
            else if(grid[main.getRow()][main.getCol()- 1] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(main.getRow() == goby[i].getRow() && main.getCol()- 1 == goby[i].getCol()){
                                displayAttack = true;
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                }
                            }
                    }
            }
            else{
                displayAttack = false;
            }

        if(displayAttack){
            cout << monsterAttack;
        }
        clearScreen();
        displayGrid();
    }
    else {
        itemCollected = "";
        showGrid = false;
        equipItem = false;
        

        // Restore the previous position if it was the `>`
        if (lastVal == '>') {
         grid[lastAdvancerPos.r()][lastAdvancerPos.c()] = '>';
         }
         // Restore the previous position if it was the `?`
         else if (lastVal == '?'){
         grid[lastAdvancerPos.r()][lastAdvancerPos.c()] = '?';
         }
         // Restore the previous position if it was the `)`
         else if (lastVal == ')'){
         grid[lastAdvancerPos.r()][lastAdvancerPos.c()] = ')';
         }
         // Restore the previous position if it was the `)`
         else if (lastVal == '&'){
         grid[lastAdvancerPos.r()][lastAdvancerPos.c()] = '&';
         }
         else {
        grid[main.getRow()][main.getCol()] = lastVal; // Clear current position
        }
        // Update player position based on direction
        if (trueWithProbability(0.1)) {
            main.sethitPoints(main.gethitPoints() + 1);
        }
        
        int newRow = main.getRow();
        int newCol = main.getCol();
        
        //when a player uses the up command
        if (direction == ARROW_UP && newRow > 0 && grid[newRow - 1][newCol] != '#') {
            if(grid[newRow - 1][newCol] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(newRow - 1 == snaky[i].getRow() && newCol == snaky[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), snaky[i].getDexterity(), snaky[i].getArmor(), playerAttack, "Snakewoman", "Player")){
                                    snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0,main.getStrength() + main.getWeapon().getDamageAmount() - 1));
                                } //snakewoman hit points are updated if the main's weapon hits it
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    //player hit points are updated if the snakewoman's weapon hits it
                                    int playerSleep = randInt(2,6); //used to occasionally put the player to sleep
                                    if(trueWithProbability(0.2)){
                                        monsterAttack += ", putting Player to sleep";
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }
                                }
                                if(snaky[i].gethitPoints() <= 0){
                                    snaky.erase(snaky.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.333333)){ //1/3 chance of spawning a weapon when the snajewoman dies
                                        grid[newRow - 1][newCol] = ')';
                                        snakewomanObjects.push_back(Coord(newRow-1,newCol));
                                    }
                                    else{
                                        grid[newRow - 1][newCol] = ' ';
                                    }
                                }
                            }
                    }
            }
            else if(grid[newRow-1][newCol] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(newRow - 1== bogey[i].getRow() && newCol == bogey[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), bogey[i].getDexterity(), bogey[i].getArmor(), playerAttack, "Bogeyman", "Player")){
                                    bogey[i].sethitPoints(snaky[i].gethitPoints() - randInt(0,main.getStrength() + main.getWeapon().getDamageAmount() - 1));
                                } //bogeyman hit points are updated if the player's weapon hits it
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                } //player hit points are updated if the bogeyman's weapon hits it
                                if(bogey[i].gethitPoints() <= 0){
                                    grid[newRow - 1][newCol] = ' ';
                                    bogey.erase(bogey.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.1)){ //1/10 chance of placing a weapon when dead
                                        grid[newRow - 1][newCol] = ')';
                                        bogeymanObjects.push_back(Coord(newRow - 1, newCol));
                                    }
                                    else{
                                        grid[newRow - 1][newCol] = ' ';
                                    }
                                }
                            }
                    }
            }
            else if(grid[newRow-1][newCol] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(newRow - 1 == dragy[i].getRow() && newCol == dragy[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), dragy[i].getDexterity(), dragy[i].getArmor(), playerAttack, "Dragon", "Player")){
                                    dragy[i].sethitPoints(snaky[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                } //dragon hit points are updated if the main's weapon hits it
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                } //player hit points are updated if the dragon's weapon hits it
                                if(dragy[i].gethitPoints() <= 0){
                                    grid[newRow-1][newCol] = '?'; //places a scroll whenever a dragon dies
                                    dragonObjects.push_back(Coord(newRow-1,newCol));
                                    dragy.erase(dragy.begin() + i);
                                    monsterAttack = "";
                                }
                                
                            }
                    }
            }
            else if(grid[newRow - 1][newCol] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(newRow - 1 == goby[i].getRow() && newCol == goby[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), goby[i].getDexterity(), goby[i].getArmor(), playerAttack, "Goblin", "Player")){
                                    goby[i].sethitPoints(goby[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                } //goblin hit points are updated if the player's weapon hits it
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                } //player hit points are updated if the goblin's weapon hits it
                                if(goby[i].gethitPoints() <= 0){
                                    goby.erase(goby.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.333333)){ //1/3 chance of placing a weapon upon death
                                        grid[newRow - 1][newCol] = ')';
                                        goblinObjects.push_back(Coord(newRow-1,newCol));
                                    }
                                    else{
                                        grid[newRow - 1][newCol] = ' ';
                                    }
                                }
                                
                            }
                    }
            }
            else{
                newRow--; //if the player is not moving into a monster, the row is simply updated
                displayAttack = false;
                
            }

        }
        //the code for the three other directions (left, right, down) are below. These below functions simply parallel the ones above, but are slightly modified to account for the different direction. As a result, there will be limited pseudocode in these sections
        else if (direction == ARROW_DOWN && newRow < 17 && grid[newRow + 1][newCol] != '#') {
            if(grid[newRow + 1][newCol] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(newRow + 1 == snaky[i].getRow() && newCol == snaky[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), snaky[i].getDexterity(), snaky[i].getArmor(), playerAttack, "Snakewoman", "Player")){
                                    snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){
                                        monsterAttack += ", putting Player to sleep";
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }
                                }
                                if(snaky[i].gethitPoints() <= 0){
                                    snaky.erase(snaky.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.3333333)){
                                        grid[newRow+1][newCol] = ')';
                                        snakewomanObjects.push_back(Coord(newRow+1,newCol));
                                    }
                                    else{
                                        grid[newRow][newCol + 1] = ' ';
                                    }
                                }

                                
                            }
                    }
            }
            else if(grid[newRow+1][newCol] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(newRow + 1== bogey[i].getRow() && newCol == bogey[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), bogey[i].getDexterity(), bogey[i].getArmor(), playerAttack, "Bogeyman", "Player")){
                                    bogey[i].sethitPoints(bogey[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(bogey[i].gethitPoints() <= 0){
                                    bogey.erase(bogey.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.1)){
                                        grid[newRow + 1][newCol] = ')';
                                        bogeymanObjects.push_back(Coord(newRow + 1, newCol));
                                    }
                                    else{
                                        grid[newRow + 1][newCol] = ' ';
                                    }
                                }
                                
                            }
                    }
            }
            else if(grid[newRow+1][newCol] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(newRow + 1 == dragy[i].getRow() && newCol == dragy[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), dragy[i].getDexterity(), dragy[i].getArmor(), playerAttack, "Dragon", "Player")){
                                    dragy[i].sethitPoints(dragy[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].gethitPoints() <= 0){
                                    grid[newRow+1][newCol] = '?';
                                    dragonObjects.push_back(Coord(newRow+1,newCol));
                                    dragy.erase(dragy.begin() + i);
                                    monsterAttack = "";
                                }
                                
                            }
                    }
            }
            else if(grid[newRow + 1][newCol] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(newRow + 1 == goby[i].getRow() && newCol == goby[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), goby[i].getDexterity(), goby[i].getArmor(), playerAttack, "Goblin", "Player")){
                                    goby[i].sethitPoints(goby[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(goby[i].gethitPoints() <= 0){
                                    goby.erase(goby.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.333333)){
                                        grid[newRow + 1][newCol] = ')';
                                        goblinObjects.push_back(Coord(newRow+1,newCol));
                                    }
                                    else{
                                        grid[newRow + 1][newCol] = ' ';
                                    }
                                }
                                
                            }
                    }
            }
            else{
                newRow++;
                displayAttack = false;
            }
        }
        else if (direction == ARROW_LEFT && newCol > 0 && grid[newRow][newCol - 1] != '#'/* && grid[newRow][newCol - 1] != 'D' && grid[newRow][newCol - 1] != 'G' && grid[newRow][newCol - 1] != 'S' && grid[newRow][newCol - 1] != 'B'*/) {
            if(grid[newRow][newCol - 1] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(newRow == snaky[i].getRow() && newCol - 1 == snaky[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), snaky[i].getDexterity(), snaky[i].getArmor(), playerAttack, "Snakewoman", "Player")){
                                    snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){
                                        monsterAttack += ", putting Player to sleep";
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }
                                }
                                if(snaky[i].gethitPoints() <= 0){
                                    snaky.erase(snaky.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.3333333)){
                                        grid[newRow][newCol - 1] = ')';
                                        snakewomanObjects.push_back(Coord(newRow,newCol-1));
                                    }
                                    else{
                                        grid[newRow][newCol - 1] = ' ';
                                    }
                                }
                                
                            }
                    }
            }
            else if(grid[newRow][newCol - 1] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(newRow == bogey[i].getRow() && newCol - 1 == bogey[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), bogey[i].getDexterity(), bogey[i].getArmor(), playerAttack, "Bogeyman", "Player")){
                                    bogey[i].sethitPoints(bogey[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(bogey[i].gethitPoints() <= 0){
                                    bogey.erase(bogey.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.1)){
                                        grid[newRow][newCol- 1] = ')';
                                        bogeymanObjects.push_back(Coord(newRow, newCol- 1));
                                    }
                                    else{
                                        grid[newRow][newCol- 1] = ' ';
                                    }
                                }
                                
                            }
                    }
            }
            else if(grid[newRow][newCol - 1] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(newRow == dragy[i].getRow() && newCol - 1 == dragy[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), dragy[i].getDexterity(), dragy[i].getArmor(), playerAttack, "Dragon", "Player")){
                                    dragy[i].sethitPoints(dragy[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].gethitPoints() <= 0){
                                    grid[newRow][newCol - 1] = '?';
                                    dragonObjects.push_back(Coord(newRow,newCol-1));
                                    dragy.erase(dragy.begin() + i);
                                    monsterAttack = "";
                                }
                                
                            }
                    }
            }
            else if(grid[newRow][newCol - 1] == 'G'){
                    for (int i = 0; i < goby.size(); i++) {
                            if(newRow == goby[i].getRow() && newCol - 1 == goby[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), goby[i].getDexterity(), goby[i].getArmor(), playerAttack, "Goblin", "Player")){
                                    goby[i].sethitPoints(goby[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(goby[i].gethitPoints() <= 0){
                                    goby.erase(goby.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.333333)){
                                        grid[newRow][newCol-1] = ')';
                                        goblinObjects.push_back(Coord(newRow,newCol-1));
                                    }
                                    else{
                                        grid[newRow][newCol-1] = ' ';
                                    }
                                }
                            }
                    }
            }
            
            else{
                newCol--;
                displayAttack = false;
            }
        }
        else if (direction == ARROW_RIGHT && newCol < 69 && grid[newRow][newCol + 1] != '#'/* && grid[newRow][newCol + 1] != 'D' && grid[newRow][newCol + 1] != 'G' && grid[newRow][newCol + 1] != 'S' && grid[newRow][newCol + 1] != 'B'*/) {
            if(grid[newRow][newCol + 1] == 'S'){
                    for (int i = 0; i < snaky.size(); i++) {
                            if(newRow == snaky[i].getRow() && newCol + 1 == snaky[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), snaky[i].getDexterity(), snaky[i].getArmor(), playerAttack, "Snakewoman", "Player")){
                                    snaky[i].sethitPoints(snaky[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(snaky[i].getWeapon().hit(snaky[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Snakewoman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + snaky[i].getWeapon().getDamageAmount() - 1));
                                    int playerSleep = randInt(2,6);
                                    if(trueWithProbability(0.2)){
                                        monsterAttack += ", putting Player to sleep";
                                        if(main.getSleepTime() == 0){
                                            main.setSleepTime(playerSleep);
                                        }
                                        else{
                                            if(main.getSleepTime() < playerSleep){
                                                main.setSleepTime(playerSleep);
                                            }
                                        }
                                    }
                                }
                                if(snaky[i].gethitPoints() <= 0){
                                    snaky.erase(snaky.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.3333333)){
                                        grid[newRow][newCol + 1] = ')';
                                        snakewomanObjects.push_back(Coord(newRow,newCol+1));
                                    }
                                    else{
                                        grid[newRow][newCol + 1] = ' ';
                                    }
                                }

                                
                            }
                    }
            }
            else if(grid[newRow][newCol + 1] == 'G'){
                for (int i = 0; i < goby.size(); i++) {
                    if(newRow == goby[i].getRow() && newCol + 1 == goby[i].getCol()){
                        displayAttack = true;
                        if(main.getWeapon().hit(main.getDexterity(), goby[i].getDexterity(), goby[i].getArmor(), playerAttack, "Goblin", "Player")){
                            goby[i].sethitPoints(goby[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                        }
                        if(goby[i].getWeapon().hit(goby[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Goblin")){
                            main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + goby[i].getWeapon().getDamageAmount() - 1));
                        }
                        if(goby[i].gethitPoints() <= 0){
                            goby.erase(goby.begin() + i);
                            monsterAttack = "";
                            if(trueWithProbability(0.333333)){
                                grid[newRow][newCol+1] = ')';
                                goblinObjects.push_back(Coord(newRow,newCol+1));
                            }
                            else{
                                grid[newRow][newCol+1] = ' ';
                            }
                        }
                    }
                }
            }
            else if(grid[newRow][newCol + 1] == 'D'){
                    for (int i = 0; i < dragy.size(); i++) {
                            if(newRow == dragy[i].getRow() && newCol + 1 == dragy[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), dragy[i].getDexterity(), dragy[i].getArmor(), playerAttack, "Dragon", "Player")){
                                    dragy[i].sethitPoints(dragy[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].getWeapon().hit(dragy[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Dragon")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + dragy[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(dragy[i].gethitPoints() <= 0){
                                    grid[newRow][newCol + 1] = '?';
                                    dragonObjects.push_back(Coord(newRow,newCol+1));
                                    dragy.erase(dragy.begin() + i);
                                    monsterAttack = "";
                                }
                                
                            }
                    }
            }
            else if(grid[newRow][newCol + 1] == 'B'){
                    for (int i = 0; i < bogey.size(); i++) {
                            if(newRow == bogey[i].getRow() && newCol + 1 == bogey[i].getCol()){
                                displayAttack = true;
                                if(main.getWeapon().hit(main.getDexterity(), bogey[i].getDexterity(), bogey[i].getArmor(), playerAttack, "Bogeyman", "Player")){
                                    bogey[i].sethitPoints(bogey[i].gethitPoints() - randInt(0,main.getStrength()  + main.getWeapon().getDamageAmount() - 1));
                                }
                                if(bogey[i].getWeapon().hit(bogey[i].getDexterity(), main.getDexterity(), main.getArmor(), monsterAttack, "Player", "Bogeyman")){
                                    main.sethitPoints(main.gethitPoints() - randInt(0,main.getStrength() + bogey[i].getWeapon().getDamageAmount() - 1));
                                }
                                if(bogey[i].gethitPoints() <= 0){
                                    bogey.erase(bogey.begin() + i);
                                    monsterAttack = "";
                                    if(trueWithProbability(0.1)){
                                        grid[newRow][newCol+ 1] = ')';
                                        bogeymanObjects.push_back(Coord(newRow, newCol+ 1));
                                    }
                                    else{
                                        grid[newRow][newCol+ 1] = ' ';
                                    }
                                }

                                
                            }
                    }
            }
            else{
                newCol++;
                displayAttack = false;
            }
        }
        else if (direction == 'c') { // Cheat code
            main.setStrength(9);
            main.setmaxhitPoints(50);
            main.sethitPoints(50);
        } //allows player to cheat to help testing
        
        else if (direction == 'g' && inventSize < 25){
            int count = 0; //used to see if gameobject was dropped by a monster
            for(int i = 0; i < goblinObjects.size(); i++){
                if(snakewomanObjects[i].r() == lastAdvancerPos.r() && snakewomanObjects[i].c() == lastAdvancerPos.c() && grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == ')'){ //checks if weapon was dropped by the snakewoman
                    inventory.push_back(new MagicFangsOfSleep());
                    itemCollected = "You pick up magic fangs of sleep";
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                    count++;
                }
            }
            for(int i = 0; i < bogeymanObjects.size(); i++){
                if(bogeymanObjects[i].r() == lastAdvancerPos.r() && bogeymanObjects[i].c() == lastAdvancerPos.c() && grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == ')'){ //checks if weapon was dropped by the bogeyman
                    inventory.push_back(new MagicAxe()); //magic axe is dropped
                    itemCollected = "You pick up magic axe";
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                    count++;
                }
            }

            for(int i = 0; i < goblinObjects.size(); i++){
                if(goblinObjects[i].r() == lastAdvancerPos.r() && goblinObjects[i].c() == lastAdvancerPos.c() && grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == ')'){ //checks if the weapon was dropped by the goblin
                    int chance = randInt(1,2); //used to make it so either a magic axe of magic fangs of sleep is dropped
                    if(chance == 1){
                        inventory.push_back(new MagicAxe());
                        itemCollected = "You pick up magic axe";
                    }
                    else{
                        inventory.push_back(new MagicFangsOfSleep());
                        itemCollected = "You pick up magic fangs of sleep";
                    }
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                    count++;
                }
            }
            

            for(int i = 0; i < dragonObjects.size(); i++){
                if(dragonObjects[i].r() == lastAdvancerPos.r() && dragonObjects[i].c() == lastAdvancerPos.c() && grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == '?'){ //checks if the scroll was dropped by a dragon
                    int chance = randInt(1,5); //used to randomize which scroll is dropped
                    if(chance == 1){
                        inventory.push_back(new ScrollOfImproveArmor());
                        itemCollected = "You pick up a scroll of improve armor";
                    }
                    else if(chance == 2){
                        inventory.push_back(new ScrollOfRaiseStrength());
                        itemCollected = "You pick up a scroll of raise strength";
                    }
                    else if(chance == 3){
                        inventory.push_back(new ScrollOfTeleportation());
                        itemCollected = "You pick up a scroll of scroll of teleportation";
                    }
                    else if(chance == 4){
                        inventory.push_back(new ScrollOfEnhanceHealth());
                        itemCollected = "You pick up a scroll of enhance health";
                    }
                    else{
                        inventory.push_back(new ScrollOfEnhanceDexterity());
                        itemCollected = "You pick up a scroll of enhance dexterity";
                    }
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                    count++;
                }
            }
            if(count == 0){ //when the above if statments do not execute, we know that the scroll/weapon was placed on the map when the level was made
                if(grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == ')'){
                    int chance = randInt(1,3); //randomizes if weapon is maze or long sword or short sword
                    if(chance == 1){
                        inventory.push_back(new Mace());
                        itemCollected = "You pick up mace";
                    }
                    else if(chance == 2){
                        inventory.push_back(new LongSword());
                        itemCollected = "You pick up long sword";
                    }
                    else{
                        inventory.push_back(new ShortSword());
                        itemCollected = "You pick up short sword";
                    }
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                }
                else if(grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == '?'){
                    int chance = randInt(1,4); //randomize which scroll is picked up (all except scroll of teleportation)
                    if(chance == 1){
                        inventory.push_back(new ScrollOfImproveArmor());
                        itemCollected = "You pick up a scroll of improve armor";
                    }
                    else if(chance == 2){
                        inventory.push_back(new ScrollOfRaiseStrength());
                        itemCollected = "You pick up a scroll of raise strength";
                    }
                    else if(chance == 3){
                        inventory.push_back(new ScrollOfEnhanceHealth());
                        itemCollected = "You pick up a scroll of enhance health";
                    }
                    else{
                        inventory.push_back(new ScrollOfEnhanceHealth());
                        itemCollected = "You pick up a scroll of enhance dexterity";
                    }
                    grid[main.getRow()][main.getCol()] = ' ';
                    inventSize++;
                }
                else if(grid[lastAdvancerPos.r()][lastAdvancerPos.c()] == '&'){
                    win = true; //if player wins golden idol, they win
                }
            }
        }
        else if (direction == 'g'){
            itemCollected = "Your knapsack is full; you can't pick that up."; //no longer able to pick up item
        }
        else if (direction == '>' && main.getRow() == exitTracker.r() && main.getCol() == exitTracker.c()) { // Create a new level
                level++; //advances level
                empty.clear();
                initGrid(); // Reinitialize the grid for a new level
                displayGrid();
                return;
        }
        /*else if(direction == 'p'){ //cheat command used during personal testing
            level++;
            empty.clear();
            initGrid(); // Reinitialize the grid for a new level
            displayGrid();
            return;
        }*/

        

        
        // Store the new position's value before moving
        lastVal = grid[newRow][newCol];
        if (lastVal == '>' || lastVal == ')' || lastVal == '?'|| lastVal == '&') {
            lastAdvancerPos = Coord(newRow, newCol); // Store the position of `>`
        }

        // Update player position
        main.setRow(newRow);
        main.setCol(newCol);

        // Set new position
        grid[main.getRow()][main.getCol()] = '@';

        // Move enemies towards player
        for (int i = 0; i < bogey.size(); i++) {
            bogey[i].moveTowardPlayer(main.getRow(), main.getCol(), grid);
        } //iterates through bogeymen vector and has them advance to player if applicable
        for (int i = 0; i < snaky.size(); i++) {
            snaky[i].moveTowardPlayer(main.getRow(), main.getCol(), grid);
        } //iterates through snakewomen vector and has them advance to player if applicable
        for (int i = 0; i < goby.size(); i++) {
            goby[i].moveTowardPlayer(main.getRow(), main.getCol(), grid);
        } //iterates through goblin vector and has them advance to player if applicable
        clearScreen();
        displayGrid();
        if(win){
            cout << "You pick up the golden idol" << endl << "Congratulations, you won!" << endl;
        }
        if(displayAttack){
            cout << playerAttack <<  endl << monsterAttack << endl;
        }
        if(itemCollected != ""){
            cout << itemCollected << endl;
        }
        if(win || main.gethitPoints() <= 0){
            cout << "Press q to exit game." << endl;
        } //player is prompted to exit game when they win or have 0 hit points
    }
}
void Level::displayGrid(){
    for(int i=0; i < 18; i ++){
        for(int j=0; j < 70; j ++){
            cout << grid[i][j]; //iterates through grid bounds and prints each element
        }
        cout << endl;
    } 
    main.displayStats(level); //displays player stats underneath grid
    cout << endl;
}


//getter methods
int Level::mainCol(){
    return main.getCol();
}
int Level::mainRow(){
    return main.getRow();
}

int Level::getLevel(){
    return level;
}
char Level::getGrid(int row, int col){
    return grid[row][col];
}
//setter method
void Level::setGrid(int row, int col, char cha){
    grid[row][col] = cha;
}

//iterates through and prints the contents of the inventory
void Level::displayInventory(){
    string alp[25] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","t","u","v","w","x","y","z"};
    cout << "Inventory:" << endl;
    for(int i = 0; i < inventSize; i++){
        cout << alp[i] << ". " << inventory[i]->getName(); //prints out inventory by iterating through the inventory vector
        cout << endl;
    }
}

