#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <algorithm> // for shuffle
#include <random>    // for mt19937 (Mersenne Twister engine) 

class Deck
{
public:
    Deck();
    Deck(int numberOfDecks);
    Deck(int numberOfCards, bool custom);
    ~Deck();
    void  makeDeck();
    void  makeDeck(int numberOfCards);
    void  shuffle();
    Card* deal();
    Card* dealCardAt(int position);
    void  printDeck();
    int   cardsLeft();
    void  clearDeck(int numberOfDecks);
    void  eraseDeck();
    void  addCard(Card* pCard);
    void  printTopCard();
    void  addCardAt(Card* pCard, int slot);
    int   deckSize();
    void  recycleDeck(Deck* draw);
    void  resetState();
    Card* getTopDeckCard();
    Card* getDeckCardAt(int slot);

private:
    std::vector<Card*> m_deck;  //this is the vector that will hold the deck
};
#endif // DECK_H
