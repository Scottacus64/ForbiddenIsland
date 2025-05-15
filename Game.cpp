#include "Game.h"


using namespace std;
const vector <int> Game::invalidSquares = {0,1,4,5,6,11,24,29,30,31,34,35};


Game::Game()
{
    islandDeck = Deck();
    floodDeck = Deck();
    floodDiscard = Deck(0);
    floodOut = Deck(0);
    islandHand = Hand();
    islandOut = Hand();
    treasureDeck = Deck(28, false);
    treasureDiscard = Deck(0);
    validSquares = {2,3,7,8,9,10,12,13,14,15,16,17,18,19,20,21,22,23,25,26,27,28,32,33};
    newGame();
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
    if(floodDeck.deckSize() == 0)
    {
        floodDiscard.recycleDeck(&floodDeck);
    }
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
        Player newPlayer = Player(classValue, slot);
        for (int i=0; i<islandHand.getSize(); i++)
        {
            Card* pCard = islandHand.getCard(i);
            int cardValue = pCard->getCharacterValue();
            if (classValue == cardValue)
            {
                newPlayer.placePlayer(validSquares[i]);
            }
        }
        players.push_back(newPlayer);
    }
}


int Game::getPlayerStartLocation(Player& player)
{
    for (int i=0; i<islandHand.getSize(); i++)
    {
        Card* pCard = islandHand.getCard(i);
        if (player.getPlayerClass() == pCard->getCharacterValue())
        {
            return i;
        }
    }
    return -1;
}


void Game::getTreasure(Player& player, int treasure)
{
    Card* pCard;
    bool canGet = player.canGetTreasure(treasure);
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
        islandTreasure.erase(remove(islandTreasure.begin(), islandTreasure.end(), treasure), islandTreasure.end());
        playerTreasure.push_back(treasure);
        cout << "player treasure = ";
        for (int i=0; i<playerTreasure.size(); i++)
        {
            cout << playerTreasure[i] << " ";
        }
        cout << endl;
        cout << "island treasure = ";
        for (int i=0; i<islandTreasure.size(); i++)
        {
            cout << islandTreasure[i] << " ";
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
    Card* pCard = givePlayer.giveTreasureCard(cardSlot);
    takePlayer.getTreasureCard(pCard);
}


void Game::movePlayer(Player& player, int  direction)
{
    if (direction == 0 or direction == 2 or direction == 4 or direction == 6 or player.getPlayerClass() == 2)
    {
        bool result = checkValidMove(player.getLocation(), direction);
        if (result == true)
        {
            player.setLocation(direction);
        }
        cout <<  "players actions = " << player.getActions() << endl;
    }
    else
    {
        cout << "can't move that direction" << endl;
    }
}



void Game::heloPlayers(int location)
{
    vector<Player*>startPlayers;
    for(int i=0; i<players.size(); i++)
    {
        Player& p = players[i];
        if (p.getLocation() == location)
        {
            startPlayers.push_back(&p);
        }
    }
    cout << "These players are in this location: ";
    for (int i=0; i<startPlayers.size(); i++)
    {
        cout << startPlayers[i]->getPlayerClass() << ", ";
    }
    cout << endl;
    for (int i=0; i<startPlayers.size(); i++)
    {
        char move;
        cout << "do you want to move player " << startPlayers[i]->getPlayerClass() << " (y/n)" ;
        cin >> move;
        if (move != 'Y' && move != 'y')
        {
            startPlayers.erase(startPlayers.begin() + i);
        }
    }
    cout << "startPlayers = ";
    for (int i=0; i<startPlayers.size(); i++)
    {
        cout << startPlayers[i]->getPlayerClass() << ", ";
    }
    cout << endl;
    cout << "where to helo to: ";
    int destination;
    cin >> destination;
    for (int i=0; i<startPlayers.size(); i++)
    {
        startPlayers[i]->fly(destination);
    }
    updatePlayerLocations();
}


void Game::updatePlayerLocations()
{
    playerLocations.clear();
    for(int i=0; i<players.size(); i++)
    {
        Player& p = players[i];
        playerLocations.push_back(p.getLocation());
        cout << "player " << p.getPlayerClass() << " is at " << p.getLocation() << endl;
    }
}


bool Game::checkForWin()
{
    bool win = true;
    int flLocation = 0;
    for(int i=0; i<24; i++)
    {
        if (islandCardPositions[i].first == 2)
        flLocation = i;
    }
    cout << "fools landing at: " << flLocation << endl;
    for (int i=0; i<players.size(); i++)
    {
        if(players[i].getLocation() != flLocation)
        {
            win = false;
        }
    }
    if(playerTreasure.size() < 4)
    {
        win = false;
    }
}


bool Game::checkForLoss()
{
    int fire = 0;
    int water = 0;
    int wind = 0;
    int earth = 0;
    bool loss = false;
    for(int i=0; i<islandHand.getSize(); i++)
    {
        Card* card = islandHand.getCard(i);
        if(card->getTreasureValue() == 1 and card->getState() > 0){fire +=1;}
        if(card->getTreasureValue() == 2 and card->getState() > 0){water +=1;}
        if(card->getTreasureValue() == 3 and card->getState() > 0){wind +=1;}
        if(card->getTreasureValue() == 4 and card->getState() > 0){earth +=1;}
    }
    if((fire == 0 && find(playerTreasure.begin(), playerTreasure.end(), 1) == playerTreasure.end())){loss = true;}
    if((water == 0 && find(playerTreasure.begin(), playerTreasure.end(), 2) == playerTreasure.end())){loss = true;}
    if((wind == 0 && find(playerTreasure.begin(), playerTreasure.end(), 3) == playerTreasure.end())){loss = true;}
    if((earth == 0 && find(playerTreasure.begin(), playerTreasure.end(), 4) == playerTreasure.end())){loss = true;}
    return loss;
}


void Game::printGameState()
{
    vector<string>classes = {"engineer ", "expolorer", "pilot   ", "navigator", "diver   ", "messenger"};
    islandHand.printHand(1);

    cout << "Active player: " << activePlayer << "      Water level: " << waterLevel << endl;
    for (int i=0; i<players.size(); i++)
    {
        Player& player = players[i];
        int playerClass = player.getPlayerClass();
        int location = player.getLocation();
        cout << "Player " << i << ": " << classes[playerClass-1] << "\t" << " location: " << location <<  "\t" << "treasure: ";
        player.printHand();
    }
    cout << "\nChoose an option:"  << "\n" << "1) Move \n" << "2) Shore Up \n" << "3) Get Treasure \n" << "4) Play Card \n" << "5) Special \n";
    // characters: 1 engineer BG, 2 expolorer CG, 3 pilot FL, 4 nav GG, 5 diver IG, 6 messenger SG,
    // treasure: 1 fire, 2 water, 3 wind, 4 earth, 5 helo, 6 sandbag, 7 water rise
}


void Game::newGame()
{
    int numberOfPlayers = 0;
    Card* pCard;
    // rebuild and shuffle the decks
    islandDeck.resetState();
    islandDeck.shuffle();
    for(int i=0; i<floodDiscard.deckSize(); i++)
    {
        pCard = floodDiscard.deal();
        floodDeck.addCard(pCard);
    }
    floodDeck.shuffle();
    for(int i=0; i<treasureDiscard.deckSize(); i++)
    {
        pCard = treasureDiscard.deal();
        treasureDeck.addCard(pCard);
    }
    treasureDeck.shuffle();
    // create the island
    for (int i=0; i<36; i++)
    {
    if (find(validSquares.begin(), validSquares.end(), i) != validSquares.end())
        {
            pCard = islandDeck.deal();
            islandHand.addCard(pCard);
            islandCardPositions.push_back({pCard->getID(), i});
        }
        else
        {
            islandCardPositions.push_back({100,i});
        }   
    }
    // sink six island cards by one degree
    for (int i=0; i<6; i++)
    {
        flipFlood();
    }
    cout << endl;
    cout << "Enter the number of players (1–4): ";
    while (!(cin >> numberOfPlayers) || numberOfPlayers < 1 || numberOfPlayers > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between 1 and 4: ";
    }
    createPlayers(numberOfPlayers);
    for(int i=0; i<players.size(); i++)
    {
        players[i].printPlayer();
    }
    printGameState();
}