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
    createPlayers(4);
    for (int i=0; i<7; i++)
    {
        drawTreasureCards(0);
    }
    Player& givePlayer = *players[0];
    Player& takePlayer = *players[1];
    transferTreasure(givePlayer, takePlayer, 0);
    players[0]->printHand();
    players[1]->printHand();
}


Game::~Game()
{
    for (Player* p : players) {
        delete p;
    }
    players.clear();
}


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


void Game::flipFlood()
{
    Card* pCard;
    Card* pIsleCard;
    pCard = floodDeck.deal();
    int id = pCard->getID();
    pIsleCard = islandHand.getCardWithId(id);
    pIsleCard->floodCard();
    floodDiscard.addCard(pCard);  
}


void Game::shuffleFlood()
{
    floodDiscard.shuffle();
    Card* pCard;
    int deckSize = floodDiscard.deckSize();
    for (int i=0; i<deckSize; i++)
    {
        pCard = floodDiscard.deal();
        floodDeck.addCard(pCard);
    }
}


void Game::shuffleTreasure()
{
    Card* pCard;
    treasureDiscard.shuffle();
    for(int i=0; i<28; i++)
    {
        pCard = treasureDiscard.deal();
        treasureDeck.addCard(pCard);
    }
}


void Game::createPlayers(int numberOfPlayers)
{
    playerClasses = {1,2,3,4,5,6};
    int classValue;
    for (int i = 0; i < numberOfPlayers && !playerClasses.empty(); i++) 
    {    
        static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
        std::uniform_int_distribution<int> dist(0, playerClasses.size() - 1);
        int index = dist(rng);
        classValue = playerClasses[index];
        playerClasses.erase(playerClasses.begin() + index);
        int slot = i+1; 
        players.push_back(new Player(classValue, slot));
    }
    cout << "players (slot/class): ";
    Player* pPlayer;
    for (Player* p : players)
    {
        p->printPlayer();
    }
    cout << endl;
}


void Game::getTreasure(Player player, int treasure)
{
    Card* pCard;
    bool canGet = player.getTreasure(treasure);
    if (canGet == false)
    {
        cout << "You don't have four of those treasure cards" << endl;
    }
    else
    {
        for (int i=0; i<player.getHandSize(); i++)
        {
            pCard = player.discardAllTreasureOfType(treasure);
            treasureDiscard.addCard(pCard);
        }
    }
}


void Game::drawTreasureCards(int playerSlot)
{
    Player* playerUp = players[playerSlot];
    Card* pDCard;
    Card* pCard = treasureDeck.deal();
    if(pCard->getTreasureValue() == 7)
    {
        cout << "got a water rise card" << endl;
        shuffleFlood();                     //shuffle the flood discard deck and put it on top of the flood deck
        treasureDiscard.addCard(pCard);     //discard the water rise card
    }
    else
    {
        pDCard = playerUp->drawCard(pCard);
        if (pDCard != nullptr)
        {
            treasureDiscard.addCard(pDCard);
        }
    }
}


void Game::transferTreasure(Player& givePlayer, Player& takePlayer, int cardSlot)
{
    Card* pCard = givePlayer.giveTreasure(cardSlot);
    takePlayer.takeTreasure(pCard);
}