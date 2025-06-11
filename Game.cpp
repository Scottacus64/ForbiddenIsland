#include "Game.h"
#include <cstdlib>

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
}


Game::~Game()
{}


void Game::removeValidSquare(int location)
{
    validSquares.erase(remove(validSquares.begin(), validSquares.end(), location), validSquares.end());
}


void Game::printValidSquares()
{
    for (int location : validSquares)
    {
        cout << location << " ";
    }
    cout << endl;
}


int Game::flipFlood()
{
    Card* pCard;
    Card* pIsleCard;
    pCard = floodDeck.deal();                                           //  get flood deck card
    int id = pCard->getID();                                            // get the card's ID value
    pIsleCard = islandHand.getCardWithId(id);                           // use that ID to find the card from the islandHand that matches
    pIsleCard->floodCard();                                             // decrement the card's state value
    floodDiscard.addCard(pCard);                                        // add the flood deck card to the discard pile
    if(floodDeck.deckSize() == 0)                                       // if the deck gets to zero size, rebuild and shuffle
    {
        floodDiscard.recycleDeck(&floodDeck);
    }
    int cardPosition = islandHand.getCardPosition(pIsleCard);           // get the slot of the islandHand slot that the card resides at
    int location = islandHand.getLocationFromHandSlot(cardPosition);    // get the 0-36 location of the card
    cout << "\nFlooded location: " << location << "\n";
    bool loss = checkForLoss();
    if(loss == true && gameStarted == true)
    {
        cout<< "\n ***** GAME OVER *****\n";
        playAgain();
    }
    totalFlipped +=1;
    if(totalFlipped > 5){gameStarted = true;}
    return location;
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
    int numberOfCards = player.canGetTreasure(treasure);
    if (numberOfCards < 4)
    {
        cout << "You don't have four of those treasure cards" << endl;
    }
    else
    {
        for (int i=0; i<numberOfCards; i++)
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


bool Game::drawTreasureCards(int playerSlot)
{
    bool wrDrawn = false;
    Player& playerUp = players[playerSlot];
    Card* pDCard;
    Card* pCard;
    if(treasureDeck.deckSize() > 0)
    {
        pCard = treasureDeck.deal();
        cout << "Card value = " << pCard->getPrintValue() << endl;
    }
    else
    {
        treasureDiscard.shuffle();
        for(int i=0; i<treasureDiscard.deckSize(); i++)
        {
            Card* dCard = treasureDiscard.deal();
            treasureDeck.addCard(dCard);
        }
        pCard = treasureDeck.deal();
        cout << "Card value = " << pCard->getPrintValue() << endl;
    }

    if(pCard->getTreasureValue() == 7 && gameStarted == false)
    {
        treasureDeck.addCard(pCard);
        treasureDeck.shuffle();
        bool dc = drawTreasureCards(playerSlot);
    }

    if(pCard->getTreasureValue() == 7 && gameStarted == true)
    {
        cout << "got a water rise card" << endl;
        shuffleFlood();                     //shuffle the flood discard deck and put it on top of the flood deck
        treasureDiscard.addCard(pCard);     //discard the water rise card
        waterLevel +=1;
        bool loss = checkForLoss();
        if(loss == true){
            cout << "\n ***** GAME OVER *****\n";
            playAgain();
        }
        wrDrawn = true;
    }
    
    if(pCard->getTreasureValue() < 7)
    {
        playerUp.drawCard(pCard);
    }
    return wrDrawn;
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
        int result = checkValidMove(player.getLocation(), direction);
        if(player.getPlayerClass() != players[activePlayer].getPlayerClass()){result = 1;}
        if (result == 2) {player.setLocation(direction, 1);}
        if (result == 1) {player.setLocation(direction, 0);}
        if (player.getActions() < 1){nextUp = true;}
        cout <<  "players actions = " << player.getActions() << endl;
    }
    else
    {
        cout << "can't move that direction" << endl;
    }
}


int Game::checkValidMove(int location, int direction)
{
    int testLocation = destinationValue(location, direction);
    //if (testLocation == location){return 0;}
    if (testLocation < 0 or testLocation > 33) {return 0;}
    // diver check for sunken location
    if (find(invalidSquares.begin(), invalidSquares.end(), testLocation) != invalidSquares.end()){return 0;}
    if (players[activePlayer].getPlayerClass() == 2  || players[mPlayer].getPlayerClass() == 2)  //explorer
    {
        if (((location == 12 || location == 18) && direction == 5) || ((location == 17 || location ==23) && (direction ==1))){return 0;}
    }
    if ((location == 17 && direction == 2) || (location == 18 && direction == 6)){return 0;}
    if (find(validSquares.begin(), validSquares.end(), testLocation) != validSquares.end()) 
    {
        if(islandHand.getCardAt(testLocation)->getState() == 1)
        {
            if(players[activePlayer].getPlayerClass()==5)
            {
                return 1; // flooded location costs nothing
            }
            else
            {
                return 2;
            }
        }
        else
        {
            return 2;
        }
    }
    return 0;  // can't move here
}


int Game::destinationValue(int location, int direction)
{
    int testLocation = 0;
    switch (direction)
    {
        // directions 0 up, clockwise to 7 up and left, 8 for the players location for shore up
        case 0:
            testLocation = location - 6;
            break;
        case 1:
            testLocation = location - 5;
            break;
        case 2:
            testLocation = location + 1;
            break;
        case 3:
            testLocation = location + 7;
            break;
        case 4:
            testLocation = location + 6;
            break;
        case 5:
            testLocation = location + 5;
            break;
        case 6:
            testLocation = location - 1;
            break;
        case 7:
            testLocation = location - 7;
            break;
        case 8:
            testLocation = location;
            break;
    }
    return testLocation;
}


bool Game::shoreUp(int direction)
{
    int start = players[activePlayer].getLocation();
    int target = destinationValue(start, direction); 
    bool validTarget = true;
    cout << "ESU = " << engineerShoreUp << endl;
    for(int i=0; i<invalidSquares.size(); i++)
    {
        if(target == invalidSquares[i]){validTarget = false;}
    }
    if(target < 0 || target > 35){validTarget = false;}
    cout << "valid target = " << validTarget << endl;
    if(validTarget == true)
    {
        Card* pCard = islandHand.getCardAt(target);
        if (pCard->getState() == 1)
        {
            pCard->setState(2);
            if(players[activePlayer].getPlayerClass() == 1 && engineerShoreUp == false)  // check if engineer and first shore up tile
            {
                engineerShoreUp = true;
                return true;
            }
            else
            {
                engineerShoreUp = false;
                players[activePlayer].setActions(-1);
                if (players[activePlayer].getActions() < 1){nextUp = true;}
                return false;
            }
        }
        else
        {
            if(engineerShoreUp == true)     // engineer has already shored up and there is nothing else to shore up
            {
                engineerShoreUp = false;
                players[activePlayer].setActions(-1);
                if (players[activePlayer].getActions() < 1){nextUp = true;}
                cout << "Actions = " << players[activePlayer].getActions()  << endl;
                return true;
            }
        }
    }
    else
    {
        cout << "Unable to shore up location";
        return false;
    }   
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
    return win;
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
        Card* pCard = islandHand.getCard(i);
        if(pCard->getTreasureValue() == 1 && pCard->getState() > 0){fire +=1;}
        if(pCard->getTreasureValue() == 2 && pCard->getState() > 0){water +=1;}
        if(pCard->getTreasureValue() == 3 && pCard->getState() > 0){wind +=1;}
        if(pCard->getTreasureValue() == 4 && pCard->getState() > 0){earth +=1;}
        if(pCard->getPrintValue() == "FL" && pCard->getState() == 0){loss = true;}
    }
    if((fire == 0 && find(playerTreasure.begin(), playerTreasure.end(), 1) == playerTreasure.end())){loss = true;}
    if((water == 0 && find(playerTreasure.begin(), playerTreasure.end(), 2) == playerTreasure.end())){loss = true;}
    if((wind == 0 && find(playerTreasure.begin(), playerTreasure.end(), 3) == playerTreasure.end())){loss = true;}
    if((earth == 0 && find(playerTreasure.begin(), playerTreasure.end(), 4) == playerTreasure.end())){loss = true;}
    if(waterLevel > 8){loss = true;}
    if(players.size() == 0){loss = true;}
    return loss;
}


void Game::printGameState()
{
    vector<string>classes = {"engineer ", "expolorer", "pilot   ", "navigator", "diver   ", "messenger"};
    islandHand.printHand(1);

    cout << "Active player: " << activePlayer <<  "     Actions Left: " << players[activePlayer].getActions() << "      Water level: " << waterLevel << endl;
    for (int i=0; i<players.size(); i++)
    {
        Player& player = players[i];
        int playerClass = player.getPlayerClass();
        int location = player.getLocation();
        cout << "Player " << i << ": " << classes[playerClass-1] 
        << "\t" << " location: " << location <<  "\t" << "treasure: ";
        player.printHand();
    }
    cout << "player treasure = ";
    for (int i=0; i<playerTreasure.size(); i++)
    {
        cout << playerTreasure[i] << " ";
    }
    cout << "\nChoose an option:\n"
    "1) Move \n"
    "2) Shore Up \n"
    "3) Get Treasure \n"
    "4) Play Card \n"
    "5) Special \n" 
    "6) End Turn \n"
    "7) Give Treasure card \n";
    // characters: 1 engineer BG, 2 expolorer CG, 3 pilot FL, 4 nav GG, 5 diver IG, 6 messenger SG,
    // treasure: 1 fire CS,CE / 2 water CP, TP / 3 wind HG, WG / 4 earth TM,TS / 5 helo, 6 sandbag, 7 water rise
}


void Game::newGame()
{
    totalFlipped = 0;
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
    int slot = 0;
    for (int i=0; i<36; i++)
    {
    if (find(validSquares.begin(), validSquares.end(), i) != validSquares.end())
    {
        pCard = islandDeck.deal();
        islandHand.addCard(pCard);                              // islandHand is the hand from 0-23 of card pointers
        islandCardPositions.push_back({pCard->getID(), slot});     // islandCardPositions is 0-35 vector pairs of {card ID, slot}
        slot +=1;                                               // slot sets islandHand card position (0-23 to a 0-35 grid of locations
    }
    else
    {
        islandCardPositions.push_back({100,100});               // pad out islandCardPositions with an ID of 100 if not a legal spot
    }   
    }
    islandHand.printHand(1);
}


string Game::getIslandCardName(int position)
{
    int slot = islandCardPositions[position].second;
    Card* pCard = islandHand.getCard(slot);
    string name = pCard->getPrintValue();
    if(pCard->getState() == 2)
    {
        name = name + "F";
    }
    if(pCard->getState() == 1)
    {
        name = name + "B";
    }
    return name;
}


void Game::playerTurn()
{
   
}


void Game::sandBag(int player, int cardSlot, int location)
{
    Card* pCard = islandHand.getCardAt(location);
    if (pCard->getState() == 1)
    {
        pCard->setState(2);
        Card* pPlayerHandCard = players[player].playCardSlot(cardSlot);
        treasureDiscard.addCard(pPlayerHandCard);
    }
    else
    {
        cout << "Not a valid location to sandbag";
    }
}


void Game::helo(int player, int cardSlot)
{
    bool win = checkForWin();
    if(win == true)
    {
        cout << "\n******** WIN!!! *********\n";
        playAgain();
    }
    Card* pCard = players[player].playCardSlot(cardSlot);
    treasureDiscard.addCard(pCard);
}


void Game::gameTurn()
{
   
}


void Game::checkPlayerInWater()
{
    for(int i=0; i<players.size(); i++)
    {
        int pLocation = players[i].getLocation();
        bool canMove = false;
        Card* pCard = islandHand.getCardAt(pLocation);
        if(pCard->getState() == 0)
        {
            int location;
            cout << "Playert in water!";
            int pClass =players[i].getPlayerClass();
            if(pClass == 3) // pilot
            {
                canMove = true;
                cout << "choose destination to fly to: ";
                cin >> location;
                players[i].fly(location, false);
            }
            else
            {
                for(int i=0; i<8; i++)
                {
                    int destinationState = checkValidMove(pLocation,i);
                    if(destinationState > 0 && i%2 == 0)
                    {
                        canMove = true;
                    }
                    if(destinationState > 0 and players[i].getPlayerClass() == 2)
                    {
                        canMove = true;
                    }
                    if(players[i].getPlayerClass() == 5)
                    {
                        canMove = true;
                    }
                }
            }
            if(canMove == true)
            {
                cout << "Enter a direction to swim: ";
                int direction;
                cin >> direction;
                //movePlayer(players[i],direction);
            }
            else
            {
                cout << "Player lost!";
                players.erase(players.begin() + i);
                bool loss = checkForLoss();
                if(loss == true)
                {
                    cout<< "\n ***** GAME OVER *****\n";
                    playAgain();
                }
            }
        }
    }
}


void Game::nextPlayer()
{
    activePlayer +=1;
    if (activePlayer > (players.size()-1))
    {activePlayer = 0;}
    players[activePlayer].resetActions();
    nextUp = false;
}


void Game::playAgain()
{
    cout << "Do you want to play again (y/n)?";
    gameStarted = false;
    char choice;
    cin >> choice;
    if(choice == 'y' || choice == 'Y')
    {
        newGame();
    }
    else
    {
        exit(0);
    }
}


Player* Game::getPlayer(int slot)
{
    if(players.size()>slot)
    {
        Player* pPlayer = &players[slot];
        return pPlayer;
    }
    else
    {
        return nullptr;
    }
}


int Game::getPlayerTreasureCard(int player, int slot)
{
    int tValue = players[player].getCardTreasureValue(slot);
    return tValue; 
}


Deck* Game::getFloodDiscard()
{
    return &floodDiscard;
}


Deck* Game::getTreasureDiscard()
{
    return &treasureDiscard;
}


Player* Game::getActivePlayer()
{
    return &players[activePlayer];
}


int Game::getNumberOfPlayers()
{
    return players.size();
}
 

int Game::getActivePlayerSlot()
{
    return activePlayer;
}


int Game::getIslandCardFlood(int location)
{
    int slot = islandCardPositions[location].second;
    Card* pCard = islandHand.getCard(slot);
    return pCard->getState();
}


int Game::getWaterLevel()
{
    return waterLevel;
}


void Game::setWaterLevel(int wl)
{
    waterLevel = wl;
}


void Game::sendTreasure(int playerNumber, int slot)
{
    if(players[activePlayer].getHandSize() > 0)
    {
        if(playerNumber != activePlayer)
        {
            if (slot < players[activePlayer].getHandSize())
            {
                transferTreasure(players[activePlayer], players[playerNumber], slot);
            }
        }
        else {cout << "Can't transfer treasure \n";}
    }   
}


int Game::getPlayerHandSize(int player)
{
    return players[player].getHandSize();
}


bool Game::getNextUp()
{
    return nextUp;
}


void Game::setNextUp(bool nUp)
{
    nextUp = nUp;
}