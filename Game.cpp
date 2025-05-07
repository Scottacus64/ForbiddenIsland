#include "Game.h"

using namespace std;
const vector <int> Game::invalidSquares = {0,1,4,5,6,11,24,29,30,31,34,35};

Game::Game()
{
    cout << "Game class built!" << endl;
    islandDeck = Deck();
    islandDeck.shuffle();
    cout << endl;
    floodDeck = Deck();
    floodDeck.shuffle();
    floodDiscard = Deck(0);
    floodOut = Deck(0);
    islandHand = Hand();
    islandOut = Hand();
    treasureDeck = Deck(28, true);
    treasureDeck.shuffle();
    treasureDiscard = Deck(0);
    validSquares = {2,3,7,8,9,10,12,13,14,15,16,17,18,19,20,21,22,23,25,26,27,28,32,33};
    for (int i=0; i<24; i++)
    {
        Card* pCard;
        pCard = islandDeck.deal();
        islandHand.addCard(pCard);
    }
    islandHand.printHand(1);
    for (int i=0; i<6; i++)
    {
        flipFlood();
    }
    cout << endl;
    treasureDeck.printDeck();
    floodDiscard.printDeck();
    cout << endl;
    islandHand.printHand(1);
    Card* pCard;
    Card* pIsleCard;
    pCard = floodDiscard.deal();
    int id = pCard->getID();
    pIsleCard = islandHand.getCardWithId(id);
    int state = pIsleCard->getState();
    pIsleCard->floodCard();
    cout << endl;
    islandHand.printHand(1);
    shuffleFlood();
}


Game::~Game()
{}


void Game::removeValidSquare(int square)
{
    validSquares.erase(remove(validSquares.begin(), validSquares.end(), square), validSquares.end());
}


void Game::printValidSquares()
{
    for (int square : validSquares)
    {
        cout << square << " ";
    }
    cout << endl;
}


bool Game::checkValidMove(int square, int direction)
{
    int testSquare;
    // directions 0 up, clockwise to 7 up and left
    switch (direction)
    {
        case 0:
            testSquare = square - 6;
            break;
        case 1:
            testSquare = square - 5;
            break;
        case 2:
            testSquare = square + 1;
            break;
        case 3:
            testSquare = square + 7;
            break;
        case 4:
            testSquare = square + 6;
            break;
        case 5:
            testSquare = square + 5;
            break;
        case 6:
            testSquare = square - 1;
            break;
        case 7:
            testSquare = square - 7;
            break;
        default:
            return false;
    }
    cout << testSquare << endl;
    if (testSquare < 0 or testSquare > 33) {return false;}
    if (find(validSquares.begin(), validSquares.end(), testSquare) == validSquares.end())
    {
        return false;
    }
    cout << testSquare << endl;
    return true;   
}


Card* Game::flipFlood()
{
    Card* pCard;
    Card* pIsleCard;
    Card* removeCard;
    pCard = floodDeck.deal();
    int id = pCard->getID();
    pIsleCard = islandHand.getCardWithId(id);
    pIsleCard->floodCard();
    floodDiscard.addCard(pCard);  
}


void Game::shuffleFlood()
{
    floodDeck.printDeck();
    cout << endl;
    floodDiscard.printDeck();
    cout << endl;
    floodDiscard.shuffle();
    floodDiscard.printDeck();
    Card* pCard;
    int deckSize = floodDiscard.deckSize();
    for (int i=0; i<deckSize; i++)
    {
        pCard = floodDiscard.deal();
        floodDeck.addCard(pCard);
    }
    cout << endl;
    floodDeck.printDeck();
    pCard = floodDeck.deal();
    pCard->printCard();
}


void Game::shuffleTreasure()
{

}

