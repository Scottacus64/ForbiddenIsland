#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Hand.h"
using namespace std;

class Player
{
public:
    Player();
    Player(int playerClass, int slot);
    ~Player();
    void  movePlayer(int direction);
    int   shoreUp(int direction);
    bool  getTreasure(int treasure);
    void  fly(int destination);
    void  moveOtherPlayer(int player, int direection);
    Card* giveTreasure(int treasureSlot);
    void  takeTreasure(Card* pCard);
    void  printPlayer();
    int   directionValue(int direction);
    int   getHandSize();
    Card* drawCard(Card* pCard);
    Card* discardCard();
    Card* discardAllTreasureOfType(int type);
    void  nextTurn();
    void  printHand();


private:
    int  actions = 3;
    int  playerClass;
    int  square;
    int  playerSlot;
    Hand playerTreasureHand;
};

#endif 