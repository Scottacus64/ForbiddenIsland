#include "Deck.h"
#include "Player.h"
#include <iostream>

using namespace std;


Player::Player(int pc)
{
    playerClass = pc;
    actions = 3;
    square = 0;
}


Player::~Player()
{}



void Player::movePlayer(int direction)
{

}


void Player::shoreUp(int direction)
{

}


void Player::getTreasure()
{

}


void Player::fly(int destination)
{

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