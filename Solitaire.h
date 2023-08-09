#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include "Hand.h"
#include "Deck.h"
#include <iostream>

class Solitaire
{
public:
    Solitaire();
    ~Solitaire();
    void dealGame();
    void printGame();
    int  cycleDeck();
    void playGame();
    void moveToColumn(int destinationCol, int col, int slot, bool lastCard);
    void checkCanMove(Card* c, int col, int row, bool lastCard);

    Hand  getColumn(int col);
    int   getTopAce(int suit);
    Card* getColCardAt(int col, int row);
    int   getColumnSize(int col);
    Card  getTopDrawPileCard();
    Deck* getDrawPile();
    Deck* getDeck();
    Hand* getAceStack(int suit);
    int   getDrawPileSize();

    Card removeColCard(int col, int row, bool lastCard);
    void aceStackMove(int col, int row, int suit, Card* c, bool lastCard);
    void playFromDrawPile(int col);
    void playFromAces(int col, int suit);
    void reuseDeck();
    bool getWin();

private:
    Deck solitaireDeck;
    Deck drawPile;

    Hand cardCol[7];
    Hand suit[4];
    /*Hand clubs;
    Hand spades;
    Hand hearts;
    Hand diamonds;*/
    Hand Aces[4]; // = {clubs, spades, hearts, diamonds};

    std::vector<int> possibleMoves;

    bool aceFlag = false;
    int  lastCardClicked;
    int  dpSize;
    int  cardCycle;
    bool win;

};
#endif // SOLITAIRE_H