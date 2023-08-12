#ifndef HAND_H
#define HAND_H

#include "Card.h"

class Hand
{
public:
    Hand();
    ~Hand();
    void printHand();
    Card removeCard(int position);
    void addCard(Card card);
    void clearHand();

    Card* getCard(int position);
    int getSize();
    int getLastCardID();
    char getLastCardSuit();

    void sortHandValue();
    void sortActualValue();
    void sortHandSuit();
    void sortHandSuitAndValue();

    static bool compareByCardValue(Card& card1, Card& card2);
    static bool compareByCardActualValue(Card& card1, Card& card2);
    static bool compareBySuit(Card& card1, Card& card2);
    static bool compareBySuitAndCardValue(Card& card1, Card& card2);



private:
    std::vector<Card> m_hand;
};

#endif // HAND_H