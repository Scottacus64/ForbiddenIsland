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
    playerTreasure = Hand();
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
}


bool Player::getTreasure(int treasure)
{
// if true then need game to get player to discard these cards
    int numberOfCards = playerTreasure.countValue(treasure);
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
    }
}