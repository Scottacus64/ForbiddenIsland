
#include "Card.h"
#include <iostream>

const vector<string> Card::pvList = {"BG", "CG", "FL", "GG", "IG", "SG",  "CE", "CS", "CP", "TP", "HG", "WG", "TM", "TS",  "BB", "CA", "CF", "DD", "LL", "MM", "Ob", "PR", "TH", "Wt" };

//using namespace std;

Card::Card()
{  
}

Card::Card(int fv, int av, int idn, string pv, int tv, int cv)
{
    faceValue = fv;
    actualValue = av;
    id = idn;
    treasureValue = tv;
    characterValue = cv;
    printValue = pv;
    faceUp = true;
    state = 2;
    // characters: 1 engineer, 2 expolorer, 3 pilot, 4 nav, 5 diver, 6 messenger,
    // treasure: 1 fire, 2 water, 3 wind, 4 earth, 5 helo, 6 sandbag, 7 water rise
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


int Card::getState()
{
    return state;
}


void Card::printCard()
{
    if (state == 2)
    {
        std::cout << "\033[0m";
        cout << printValue;
    }
    else if (state == 1)
    {
        std::cout << "\033[31m";
        cout << printValue;
    }
    else
    {
        cout << "  ";
    }
}


void Card::printAV()
{
    cout << actualValue;
}

void Card::floodCard()
{
    state-=1;
    printCard();
    cout << " state = " << state << endl;
}


void Card::shoreUpCard()
{
    state+=1;
}

