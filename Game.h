#include <string>
#include <iostream>
#include "Deck.h"
#include "Hand.h"


using std::string;

class Game
{
public:
    Game();
    ~Game();
    void removeValidSquare(int square);
    void printValidSquares();
    bool checkValidMove(int square, int direction);
    Card* flipFlood();

private:
    Deck islandDeck;
    Deck floodDeck;
    Deck floodDiscard;
    Deck floodOut;
    Hand islandHand;
    Hand islandOut;
    static const vector<int> invalidSquares;
    vector<int> validSquares;
};