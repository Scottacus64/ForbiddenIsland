#ifndef GAME
#include <string>
#include <iostream>
#include <random> 
#include <ctime>
#include "Deck.h"
#include "Hand.h"
#include "Player.h"

using std::string;

class Game
{
public:
    Game();
    ~Game();
    void newGame();
    void removeValidSquare(int square);
    void printValidSquares();
    bool checkValidMove(int square, int direction);
    void flipFlood();
    void shuffleFlood();
    void shuffleTreasure();
    void createPlayers (int numberOfPlayers);
    void getTreasure(Player& player, int treasure);
    void drawTreasureCards(int playerSlot);
    void transferTreasure (Player& givePlayer, Player& takePlayer, int cardSlot);
    int  getPlayerStartLocation(Player& player);
    void movePlayer(Player& player, int direction);
    void heloPlayers(int location);
    void updatePlayerLocations();
    bool checkForLoss();
    bool checkForWin();
    void printGameState();

private:
    Deck islandDeck;
    Deck floodDeck;
    Deck floodDiscard;
    Deck floodOut;
    Deck treasureDeck;
    Deck treasureDiscard;
    Hand islandHand;
    Hand islandOut;
    Player generatePlayer;
    vector <Player> players;    
    static const vector<int> invalidSquares;
    vector<int> playerClasses;
    vector<int> validSquares;
    vector<pair<int, int>> islandCardPositions;
    vector<int> playerLocations;
    vector<int> islandTreasure {1,2,3,4};
    vector<int> playerTreasure;
    int waterLevel = 2;
    int activePlayer = 0;
};
#endif