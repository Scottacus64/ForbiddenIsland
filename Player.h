#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Hand.h"
using namespace std;

class Player
{
public:
    void movePlayer(int direction);
    void shoreUp(int direction);
    void getTreasure();
    void fly(int destination);
    void moveOtherPlayer(int player, int direection);
    void giveTreasure(int treasure, int player);

private:
    int actions = 3;
    int playerClass;
    Hand playerTreasure;
};

#endif 