#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include <vector>

class Hand
{
public:
    Hand();
    ~Hand();
    void  printHand(int typeOfHand);
    Card* removeCard(int position);
    Card* removeValue(int value);
    void  removeCard(Card* pCard);
    void  removeCardPointer(Card* pCard);
    void  addCard(Card* pCard);
    void  addCardAt (Card* pCard, int slot);
    void  clearHand();

    Card* getCard(int position);
    Card* getFirstFlippedUp();
    Card* getCardWithId(int id);
    int   getFirstFlippedUpPosition();
    bool  allFaceDown();
    int   getSize();
    Card* getLastCard ();
    int   getLastCardID();
    int   getLastCardValue();
    int   getCardPosition(Card* pPosCard);
    Card* getCardAt(int location);
    int   countValue(int value);
    int   getLocationFromHandSlot(int slot);

    void sortHandValue();
    void sortActualValue();

    static bool compareByCardValue(Card* pCard1, Card* pCard2);
    static bool compareByCardActualValue(Card* pCard1, Card* pCard2);


private:
    std::vector<Card*> m_hand;
    static const vector<int> invalidSquares;
    static const vector<pair<int,int>> slotLocation;
};

#endif // HAND_H