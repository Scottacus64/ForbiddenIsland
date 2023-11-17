#ifndef HAND_H
#define HAND_H

#include "Card.h"

class Hand
{
public:
    Hand();
    ~Hand();
    void printHand();
    Card* removeCard(int position);
    void  addCard(Card* pCard);
    void  addCardAt (Card* pCard, int slot);
    void  clearHand();

    Card* getCard(int position);
    Card* getFirstFlippedUp();
    int   getFirstFlippedUpPosition();
    int   getSize();
    Card* getLastCard ();
    int   getLastCardID();
    int   getLastCardValue();
    char  getLastCardSuit();
    Card* getCardAt(int location);

    void sortHandValue();
    void sortActualValue();
    void sortHandSuit();
    void sortHandSuitAndValue();

    static bool compareByCardValue(Card* pCard1, Card* pCard2);
    static bool compareByCardActualValue(Card* pCard1, Card* pCard2);
    static bool compareBySuit(Card* pCard1, Card* pCard2);
    static bool compareBySuitAndCardValue(Card* pCard1, Card* pCard2);



private:
    std::vector<Card*> m_hand;
};

#endif // HAND_H