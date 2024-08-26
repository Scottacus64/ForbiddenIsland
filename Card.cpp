
#include "Card.h"
#include <iostream>

//using namespace std;

Card::Card()
{  
}

Card::Card(int fv, int av, int idn, char s, string pv)
{
    faceValue = fv;
    actualValue = av;
    id = idn;
    suit = s;
    printValue = pv;
    faceUp = false;
}


Card::~Card()
{
}


void Card::flipCard()
{
    faceUp = !faceUp;
}


void Card::flipFaceUp()
{
    faceUp = true;
}


void Card::setFaceUp(bool fUp)
{
    faceUp = fUp;
}


int Card::getFaceValue()
{
    return faceValue;
}


int Card::getActualValue()
{
    return actualValue;
}


string Card::getPrintValue()
{
    return printValue;
}


bool Card::getFaceUp()
{
    return faceUp;
}


int Card::getID()
{
    return id;
}


char Card::getSuit()
{
    return suit;
}


int Card::getSuitInt()
{
    int suitInt;
    if (suit==67)
    {
        suitInt = 0;
    }
    else if(suit==83)
    {
        suitInt = 1;
    }
    else if(suit==72)
    {
        suitInt = 2;
    }
    else
    {
        suitInt = 3;
    }
    return suitInt;
}


int Card::getBlackRed()
{
    int blackRed;
    if (id <= 26)
    {
        blackRed = 0;
    }
    else
    {   
        blackRed = 1;
    }
    return blackRed;
}


void Card::printCard()
{
    if (faceUp == true)
    {
        if (suit == 'H' || suit == 'D')
        {
            std::cout << "\033[31m";
        }
        else
        {
            std::cout << "\033[0m";
        }
        cout << printValue << suit;
    }
    else
    {
        std::cout << "\033[0m";
        cout << " **";
    }
}


void Card::printAV()
{
    cout << actualValue;
}

