#include "Deck.h"
#include "Player.h"
#include <iostream>

using namespace std;


Player::Player()
{}

Player::Player(int pc, int slot)
{
    vector <string> classNames = {"Engineer", "Expolorer", "Pilot", "Navigator", "Diver", "Messenger"};
    playerSlot = slot;
    playerClass = pc;
    playerName = classNames[pc-1];
    actions = 3;
    location = 0;
    playerTreasureHand = Hand();
}


Player::~Player()   
{}



void Player::setLocation(int direction, int cost)
{
    int offset = directionValue(direction);
    location = location + offset;
    actions -=cost;
}


int Player::shoreUp(int direction)
{
    if (direction < 0 || direction > 7)
        return 100;
    int offset = directionValue(direction);
    int tileToShoreUp = location + offset;
    actions -=1;
    return tileToShoreUp;
}


int Player::canGetTreasure(int treasure)
{
    if(treasure>4){return 0;}
    int numberOfCards = playerTreasureHand.countValue(treasure);
    cout << "number of cards = " << numberOfCards << endl;
    return numberOfCards;
}


void Player::fly(int destination, bool helo)
{
    location = destination;
    if(helo == false){actions -=1;}
}


Card* Player::giveTreasureCard(int treasureSlot)
{
    Card* pCard = playerTreasureHand.getCard(treasureSlot);
    playerTreasureHand.removeCard(pCard);
    return pCard;
}


void Player::printPlayer()
{
    cout << playerSlot << ":" << playerClass << " ";
}


void Player::printHand()
{
    playerTreasureHand.printHand(0);
    cout << endl;
}


int Player::directionValue(int direction)
{
    switch (direction)
    {
        case 0: return -6; 
        case 1: return -5;
        case 2: return 1;
        case 3: return 7;
        case 4: return 6;
        case 5: return 5;
        case 6: return -1;
        case 7: return -7;
    }
    return 0;
}


int Player::getHandSize()
{
    int size = playerTreasureHand.getSize();
    return size;
}


void Player::drawCard(Card* pCard)
{
    Card* pDCard;
    playerTreasureHand.addCard(pCard);
}


void Player::discardCard(int slot)
{
    cout << "PlayerTreasureHand size = " << playerTreasureHand.getSize() << endl;
    cout << "PlayerSlot = " <<  playerSlot << endl;
    if(playerTreasureHand.getSize() >= (slot+1))
    {
        Card* pCard = playerTreasureHand.removeCard(slot);
        cout << endl;
        playerTreasureHand.printHand(0);
        cout << endl;
    }

}


Card* Player::playCardSlot(int slot)
{
    if(slot < playerTreasureHand.getSize())
    {
        cout << "Removing card at: " << slot << "\n";
        Card* pCard = playerTreasureHand.removeCard(slot);
        return pCard;
    }
    return nullptr;
}


Card* Player::lookAtCardSlot(int slot)
{
    Card* pCard = playerTreasureHand.getCard(slot);
    return pCard;
}


Card* Player::discardAllTreasureOfType(int type)
{
    for (int i=0; i<playerTreasureHand.getSize(); i++)
    {
        Card* pCard;
        pCard = playerTreasureHand.getCard(i);
        if (pCard->getTreasureValue() == type)
        {
            playerTreasureHand.removeCard(pCard);
            return pCard;
        }
    }
    return nullptr;
}


void Player::resetActions()
{
    actions = 3;
}


void Player::getTreasureCard(Card* pCard)
{
    playerTreasureHand.addCard(pCard);
}


int Player::getPlayerClass()
{
    return playerClass;
}


void Player::placePlayer(int loc)
{
    location = loc;
}


int Player::getLocation()
{
    return location;
}


int Player::getActions()
{
    return actions;
}


void Player::setActions(int actionsChange)
{
    if(actionsChange == 0)
    {
        actions = 0;
    }
    else
    {
        actions = actions + actionsChange;
    }
}


int Player::getCardTreasureValue(int slot)
{
    Card* pCard = playerTreasureHand.getCard(slot);
    if(pCard)
    {
        return pCard->getTreasureValue();
    }
    else
    {
        cout << "No card!" << endl;
        return 8;
    }
}


int Player::getTreasureCardSlot(Card* pCard)
{
    for(int i=0; i<playerTreasureHand.getSize(); i++)
    {
        if(pCard == playerTreasureHand.getCard(i))
        {
            return i;
        }
    }
    return 0;
}


string Player::getPlayerName()
{
    return playerName;
}