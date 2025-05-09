#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Hand.h"
using namespace std;

class Player
{
public:
    Player(int playerClass);
    ~Player();
    void movePlayer(int direction);
    void shoreUp(int direction);
    void getTreasure();
    void fly(int destination);
    void moveOtherPlayer(int player, int direection);
    void giveTreasure(int treasure, int player);
    void printPlayer();

private:
    int actions = 3;
    int playerClass;
    int square;
    Hand playerTreasure;
};

#endif 