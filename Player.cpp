#include "Deck.h"
#include "Player.h"
#include <iostream>

using namespace std;


Player::Player()
{}

Player::Player(int pc)
{
    playerClass = pc;
    actions = 3;
    square = 0;
    playerTreasureHand = Hand();
}


Player::~Player()   
{}



void Player::movePlayer(int direction)
{
    int offset = directionValue(direction);
    square = square + offset;
}


int Player::shoreUp(int direction)
{
    int offset = directionValue(direction);
    int tileToShoreUp = square + offset;
    return tileToShoreUp;
}


bool Player::getTreasure(int treasure)
{
    int numberOfCards = playerTreasureHand.countValue(treasure);
    if (numberOfCards > 3)
    {
        return true;
    }
    return false;
}


void Player::fly(int destination)
{
    square = destination;
}


void Player::moveOtherPlayer(int player, int direction)
{

}


void Player::giveTreasure(int treasure, int player)
{

}


void Player::printPlayer()
{
    cout << playerClass << " ";
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
        default: return 0;
    }
}


int Player::getHandSize()
{
    int size = playerTreasureHand.getSize();
    return size;
}


void Player::drawCard(Card* pCard)
{
    playerTreasureHand.addCard(pCard);
}


Card* Player::discardCard()
{
    playerTreasureHand.printHand(0);
    cout << "Enter 1-5 to discard a card" << endl;
    int choice;
    cin >> choice;
    Card* pCard = playerTreasureHand.removeCard(choice);
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