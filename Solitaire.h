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
    void moveToColumn(int destinationCol, int col, int slot, bool lastUnflipedCard);
    bool checkCanMove(Card* c, int col, int row, bool lastCard, bool lastUnflippedCard);
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
    int   getMoves();
    struct cardState {int ID; bool faceUp;};
    cardState sColumn[7][19];
    cardState sAces[4][13];
    cardState sSolitaireDeck[26];
    cardState sDrawPile[26];

    struct gameData {
    std::array<std::array<cardState, 19>, 7> sColumn;
    std::array<std::array<cardState, 13>, 4> sAces;
    std::array<cardState, 26> sSolitaireDeck;
    std::array<cardState, 26> sDrawPile;
    };

    gameData gameSave;
    Card removeColCard(int col, int row, bool lastCard);
    Card removeForAce(int col, int row);
    void aceStackMove(int col, int row, int suit, Card* c, bool lastCard);
    void playFromDrawPile(int col);
    void playFromAces(int col, int suit);
    void reuseDeck();
    bool getWin();
    bool checkAutoFinish();
    bool nextCard();
    void saveState();
    void printSave();

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
    int  moves;
};
#endif // SOLITAIRE_H