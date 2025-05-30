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
    void createPlayers(int numberOfPlayers);
    void newGame();
    void removeValidSquare(int location);
    void printValidSquares();
    int  checkValidMove(int location, int direction);
    int  destinationValue(int location, int direction);
    int  flipFlood();
    void shuffleFlood();
    void shuffleTreasure();
    Player* getPlayer(int slot);
    Player* getActivePlayer();
    int  getActivePlayerSlot();
    void getTreasure(Player& player, int treasure);
    void drawTreasureCards(int playerSlot);
    void transferTreasure (Player& givePlayer, Player& takePlayer, int cardSlot);
    int  getPlayerStartLocation(Player& player);
    void movePlayer(Player& player, int direction);
    bool heloPlayers(int location);
    void updatePlayerLocations();
    bool checkForLoss();
    bool checkForWin();
    void printGameState();
    void nextPlayer();
    bool shoreUp(int direction);
    void playerTurn();
    void gameTurn();
    void checkPlayerInWater();
    void sandBag(int player, int cardSlot);
    void helo(int player,int cardSlot);
    void playAgain();
    string getIslandCardName(int position);
    int   getPlayerTreasureCard(int player, int slot);
    Deck* getFloodDiscard();
    Deck* getTreasureDiscard();
    int getNumberOfPlayers();
    int getIslandCardFlood(int location);
    void setWaterLevel(int wl);
    int getWaterLevel();

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
    int waterLevel = 1;
    int activePlayer = 0;
    int mPlayer = 100;
    bool pilotFlight = false;
    bool gameStarted = false;
    int totalFlipped = 0;
    bool engineerShoreUp = false;
};
#endif