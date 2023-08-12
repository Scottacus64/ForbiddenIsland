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
    int  cycleDeck();
    void playGame();
    void moveToColumn(int destinationCol, int col, int slot, bool lastCard);
    bool checkCanMove(Card* c, int col, int row, bool lastCard);
    void printField();

    Hand  getColumn(int col);
    int   getTopAce(int suit);
    Card* getColCardAt(int col, int row);
    int   getColumnSize(int col);
    Card* getTopDrawPileCard();
    Deck* getDrawPile();
    Deck* getDeck();
    int   getDeckSize();
    Hand* getAceStack(int suit);
    int   getDrawPileSize();
    Card* getDrawPileAt(int slot);

    Card removeColCard(int col, int row, bool lastCard);
    Card removeForAce(int col, int row);
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
    Hand Aces[4]; // = {clubs, spades, hearts, diamonds};

    std::vector<int> possibleMoves;

    bool aceFlag = false;
    int  lastCardClicked;
    int  dpSize;
    int  cardCycle;
    bool win;

};
#endif // SOLITAIRE_H