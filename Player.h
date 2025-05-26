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
    void  setLocation(int direction, int cost);
    int   shoreUp(int direction);
    int   canGetTreasure(int treasure);
    void  fly(int destination);
    Card* giveTreasureCard(int treasureSlot);
    void  getTreasureCard(Card* pCard);
    int   getCardTreasureValue(int slot);
    void  printPlayer();
    void  printHand();
    int   getHandSize();
    Card* drawCard(Card* pCard);
    Card* discardCard();
    Card* playCardSlot(int slot);
    Card* lookAtCardSlot(int slot);
    Card* discardAllTreasureOfType(int type);
    void  resetActions();
    void  placePlayer(int location);
    int   getPlayerClass();
    int   getLocation();
    int   getActions();
    int   directionValue(int direction);
    void  setActions(int actionsChange);
    int   getTreasureCardSlot(Card* pCard);
    string getPlayerName();
   


private:
    int  actions = 3;
    int  playerClass;
    int  location;
    int  playerSlot;
    string playerName;
    Hand playerTreasureHand;
};

#endif 