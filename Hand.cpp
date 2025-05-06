#include "Hand.h"
#include <iostream>
const vector <int> Hand::invalidSquares = {0,1,4,5,6,11,24,29,30,31,34,35};

Hand::Hand()
{
}


Hand::~Hand()
{  
}


Card* Hand::removeCard(int position)
{
    //std::cout << "** H remove card, pos = " << position << "\n";
    Card* pCard = m_hand[position];
    m_hand.erase(m_hand.begin() + position);
    int size = m_hand.size();
    //std::cout <<"Hand size is: " << size << "\n";
    return pCard;
}


int Hand::removeCardPointer(Card* pCard)
{
    int pos = getCardPosition(pCard);
    m_hand.erase(m_hand.begin() + pos);
}


void Hand::addCard(Card* pCard)
{
    m_hand.push_back(pCard);
}


void Hand::addCardAt(Card* pCard, int slot)
{
    m_hand.insert(m_hand.begin() + slot, pCard);
}


Card* Hand::getCard(int position)
{
    Card* pCard = m_hand.at(position);
    return pCard;
}


int Hand::getSize()
{
    int size =  m_hand.size();
    return size;
}


void Hand::printHand(int typeOfHand)
{
    if (typeOfHand == 0)
        for (auto pCard : m_hand) 
        {
            pCard->printCard();
            std::cout << " ";
        }
    if (typeOfHand == 1)
    {
        int card = 0;
        for (int i=0; i<36; i++)
        {
            if (i % 6 == 0){cout << endl;}
            if((find(invalidSquares.begin(), invalidSquares.end(), i) != invalidSquares.end()))
            { cout << "   ";}
            else
            {
                Card* pCard = getCard(card);
                pCard->printCard();
                card++;
                std::cout << " ";            
            }
        }
        cout << endl << endl;
    }
}


void Hand::sortHandValue()
{
    std::sort(m_hand.begin(), m_hand.end(), compareByCardValue);
}


bool Hand::compareByCardValue(Card* pCard1, Card* pCard2) 
{
    return pCard1->getFaceValue() < pCard2->getFaceValue();
}


void Hand::sortActualValue()
{
    std::sort(m_hand.begin(), m_hand.end(), compareByCardActualValue);
}


bool Hand::compareByCardActualValue(Card* pCard1, Card* pCard2) 
{
    return pCard1->getActualValue() < pCard2->getActualValue();
}


void Hand::clearHand()
{
    m_hand.clear();
}


int Hand::getLastCardID()
{
    int id = -2;
    if (m_hand.size() > 0)
    {
        id = m_hand[m_hand.size()-1]->getID();
    }
    return id;
}


int Hand::getLastCardValue()
{
    int value = 0;
    if (m_hand.size() > 0)
    {
        value = m_hand[m_hand.size()-1]->getFaceValue();
    }
    return value;
}


Card* Hand::getLastCard()
{
    Card* pCard;
    if (m_hand.size() > 0)
    {
        pCard = m_hand[m_hand.size()-1];
    }
    return pCard;
}


Card* Hand::getFirstFlippedUp()
{
    Card* pCard;
    if (m_hand.size() > 0)
    {
        for (int i=0; i<m_hand.size(); i++)
        {
            pCard = m_hand[i];
            if (pCard->getFaceUp() == true)
            {
                break;
            }
        }
    }
    return pCard;
}


int Hand::getFirstFlippedUpPosition()
{
    Card* pCard;
    int location = -1;      // return -1 if no cards are flipped up because zero is the first slot for a column
    if (m_hand.size() > 0)
    {
        for (int i=0; i<m_hand.size(); i++)
        {
            pCard = m_hand[i];
            if (pCard->getFaceUp() == true)
            {
                location = i;
                break;
            }
        }
    }
    return location;  
}


bool Hand::allFaceDown()
{
    Card* pCard;
    bool allDown = true;
    if (m_hand.size() > 0)
    {
        for (int i=0; i<m_hand.size(); i++)
        {
            pCard = m_hand[i];
            if (pCard->getFaceUp() == true)
            {
                allDown = false;
            }
        }
    }
    return allDown;
}


Card* Hand::getCardAt(int location)
{
    Card* pCard;
    if (location < m_hand.size()){pCard = m_hand[location];}
    return pCard;

}

Card* Hand::getCardWithId(int id)
{
    Card* pCard;
    if (m_hand.size() > 0)
    {
        for (int i=0; i<m_hand.size(); i++)
        {
            pCard = m_hand[i];
            int cardID = pCard->getID();
            if (cardID == id)
            {
                return pCard;
            }
        }
    }

}

int Hand::getCardPosition(Card* pPosCard)
{
    Card* pCard;
    if (m_hand.size() > 0)
    {
        for (int i=0; i<m_hand.size(); i++)
        {
            pCard = m_hand[i];
            if (pCard == pPosCard)
            {
                return i;
            }
        }
    }
}
