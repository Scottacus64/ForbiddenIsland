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
    treasureDeck = Deck(28, false);
    treasureDeck.shuffle();
    treasureDiscard = Deck(0);
    validSquares = {2,3,7,8,9,10,12,13,14,15,16,17,18,19,20,21,22,23,25,26,27,28,32,33};
    // create the island
    for (int i=0; i<36; i++)
    {
    if (find(validSquares.begin(), validSquares.end(), i) != validSquares.end())
        {
            Card* pCard;
            pCard = islandDeck.deal();
            islandHand.addCard(pCard);
            islandCardPositions.push_back({pCard->getID(), i});
        }
        else
        {
            islandCardPositions.push_back({100,i});
        }
        
    }
    
    islandHand.printHand(1);
    // sink six island cards by one degree
    for (int i=0; i<6; i++)
    {
        flipFlood();
    }
    // print the island with sunked cards
    islandHand.printHand(1);
    // shuffle the flood discard pile, place on top of the draw pile and draw one card to remove an island card
    Card* pCard;
    Card* pIsleCard;
    pCard = floodDiscard.deal();
    int id = pCard->getID();
    pIsleCard = islandHand.getCardWithId(id);
    int state = pIsleCard->getState();
    pIsleCard->floodCard();
    cout << endl;
    islandHand.printHand(1);
    // create four players
    createPlayers(4);
    // draw six treasure cards and limit hand to five cards
    for (int i=0; i<7; i++)
    {
        drawTreasureCards(0);
    }
    // show that treasure cards can be passed to another player
    Player& givePlayer = players[0];
    Player& takePlayer = players[1];
    transferTreasure(givePlayer, takePlayer, 0);
    players[0].printHand();
    players[1].printHand();
    // this places the four players on the board
    for (int i=0; i<4; i++)
    {
        Player& spotPlayer = players[i];
        int location = placePlayers(spotPlayer);
        spotPlayer.placePlayer(validSquares[location]);
        cout << "player at: " << spotPlayer.getLocation() << endl;
    }
    // move a player in various directions and show that actions decreases with each valid move
    Player& movePlayer = players[0];
    for (int i=0; i<5; i++)
    {
        cout << endl;
        cout << "player's location is: " << movePlayer.getLocation() << " ";
        cout << "enter a direction to move: ";
        int direction;
        cin >> direction;
        cout << endl;
        gameMovePlayer(movePlayer, direction);
    }
    // fly a player to another square
    int destination;
    cout << "fly to location: ";
    cin >> destination;
    bool validDestination = find(validSquares.begin(), validSquares.end(), destination) != validSquares.end();
    if (destination != movePlayer.getLocation() && validDestination == true)
    {
        movePlayer.fly(destination);
    }
    cout << "player's new location is: " << movePlayer.getLocation() << endl;
    // move another player
    int playerToMove;
    cout << "enter a player to move (0-3)";
    cin >> playerToMove;
    Player& otherPlayer = players[playerToMove];
    cout << " player located at "  << otherPlayer.getLocation() << " enter a direction to move: ";
    int direction;
    cin >> direction;
    gameMovePlayer(otherPlayer, direction);
    cout << endl;
    cout << "player's new location is: " << otherPlayer.getLocation() << endl;

    // characters: 1 engineer BG, 2 expolorer CG, 3 pilot FL, 4 nav GG, 5 diver IG, 6 messenger SG,
    // treasure: 1 fire, 2 water, 3 wind, 4 earth, 5 helo, 6 sandbag, 7 water rise
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
    if (testSquare < 0 or testSquare > 33) {return false;}
    if (find(validSquares.begin(), validSquares.end(), testSquare) == validSquares.end())
    {
        return false;
    }
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
        players.emplace_back(classValue, slot);
    }
    cout << "players (slot/class): ";
    Player* pPlayer;
    for (Player p : players)
    {
        p.printPlayer();
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
    Player& playerUp = players[playerSlot];
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
        pDCard = playerUp.drawCard(pCard);
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


int Game::placePlayers(Player& player)
{
    for (int i=0; i<islandHand.getSize(); i++)
    {
        Card* pCard = islandHand.getCard(i);
        if (player.getPlayerClass() == pCard->getCharacterValue())
        {
            cout << player.getPlayerClass() << " : " << pCard->getCharacterValue() << " : " << i << endl;
            return i;
        }
    }
}


void Game::gameMovePlayer(Player& player, int  direction)
{
    bool result = checkValidMove(player.getLocation(), direction);
    if (result == true)
    {
        player.movePlayer(direction);
    }
    cout <<  "players actions = " << player.getActions() << endl;
}