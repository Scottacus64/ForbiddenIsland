#include "Deck.h"
#include "Player.h"
#include <iostream>

using namespace std;


Player::Player()
{}

Player::Player(int pc, int slot)
{
    playerSlot = slot;
    playerClass = pc;
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


bool Player::canGetTreasure(int treasure)
{
    if(treasure>4){return false;}
    int numberOfCards = playerTreasureHand.countValue(treasure);
    cout << "number of cards = " << numberOfCards << endl;
    if (numberOfCards > 3)
    {
        actions -=1;
        return true;
    }
    return false;
}


void Player::fly(int destination)
{
    location = destination;
    actions -=1;
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


Card* Player::drawCard(Card* pCard)
{
    Card* pDCard;
    playerTreasureHand.addCard(pCard);
    if (playerTreasureHand.getSize() > 5)
    {
        pDCard = discardCard();
        return pDCard;
    }
    else
    {
        return nullptr;
    }
}


Card* Player::discardCard()
{
    cout << endl;
    playerTreasureHand.printHand(0);
    cout << endl;
    cout << "Enter 0-5 to discard a card" << endl;
    int choice;
    cin >> choice;
    Card* pCard = playerTreasureHand.removeCard(choice);
    cout << endl;
    playerTreasureHand.printHand(0);
    cout << endl;
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


void Player::setActions(int actionsValue)
{
    actions = actionsValue;
}