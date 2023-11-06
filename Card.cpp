
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
    suit = s;
    printValue = pv;
    faceUp = false;
    id = idn;
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

