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
    void  setLocation(int direction);
    int   shoreUp(int direction);
    bool  getTreasure(int treasure);
    void  fly(int destination);
    Card* giveTreasureCard(int treasureSlot);
    void  getTreasureCard(Card* pCard);
    void  printPlayer();
    void  printHand();
    int   getHandSize();
    Card* drawCard(Card* pCard);
    Card* discardCard();
    Card* discardAllTreasureOfType(int type);
    void  resetActions();
    void  placePlayer(int location);
    int   getPlayerClass();
    int   getLocation();
    int   getActions();
    int   directionValue(int direction);


private:
    int  actions = 3;
    int  playerClass;
    int  location;
    int  playerSlot;
    Hand playerTreasureHand;
};

#endif 