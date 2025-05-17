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
    cout << "in move players" << endl;
    if (direction == 0 or direction == 2 or direction == 4 or direction == 6 or player.getPlayerClass() == 2)
    {
        int result = checkValidMove(player.getLocation(), direction);
        if (result == 2) {player.setLocation(direction, 0);}
        if (result == 1) {player.setLocation(direction, 1);}
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
    if (testLocation == location){return false;}
    if (testLocation < 0 or testLocation > 33) {return false;}
    // diver check for sunken location
    if (find(invalidSquares.begin(), invalidSquares.end(), testLocation) != invalidSquares.end()){return 0;}
    if (find(validSquares.begin(), validSquares.end(), testLocation) != validSquares.end()) 
    {
        if(players[activePlayer].getPlayerClass() == 5 ) //diver
        {
            if(islandHand.getCardAt(testLocation)->getState() == 1)
            {
                return 2; // flooded location costs nothing
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1; // all other players cost one action
        }
    }
    if (find(validSquares.begin(), validSquares.end(), testLocation) == validSquares.end()) {return 2;}
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


void Game::shoreUp()
{
    int engineerTurns = 1;
    if(players[activePlayer].getPlayerClass() == 1){engineerTurns = 2;}
    for(int i=0; i<engineerTurns; i++)
    {
        int direction;
        cout << "Enter a direction to shore up(0-7 or 8 for thr player's location):";
        cin >> direction;
        int start = players[activePlayer].getLocation();
        int target = destinationValue(start, direction); 
        Card* pCard = islandHand.getCardAt(target);
        if (pCard->getState() == 1)
        {
            pCard->setState(2);
            if(players[activePlayer].getPlayerClass() == 1 && i==0)
            {
                char another;
                cout << "Do you want to shore up another island location (y/n)?";
                cin >> another;
                if(another != 'y' && another !='Y')
                {
                    players[activePlayer].setActions(-1);
                    return;
                }
                
            }
            else
            {
                players[activePlayer].setActions(-1);
            }
        }
        else
        {
            cout << "Unable to shore up location";
        }
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

    cout << "Active player: " << activePlayer <<  "     Actions Left: " << players[activePlayer].getActions() << "      Water level: " << waterLevel << endl;
    for (int i=0; i<players.size(); i++)
    {
        Player& player = players[i];
        int playerClass = player.getPlayerClass();
        int location = player.getLocation();
        cout << "Player " << i << ": " << classes[playerClass-1] << "\t" << " location: " << location <<  "\t" << "treasure: ";
        player.printHand();
    }
    cout << "\nChoose an option:"  << "\n" << "1) Move \n" << "2) Shore Up \n" << "3) Get Treasure \n" << "4) Play Card \n" << "5) Special \n" << "6) End Turn \n";
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
    gameTurn();
}


void Game::gameTurn()
{
    pilotFlight = false;
    while(players[activePlayer].getActions()>0)
    {
        printGameState();
        int playerChoice;
        while (!(cin >> playerChoice) || playerChoice < 1 || playerChoice > 6) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 6: ";
        }
        Player& movingPlayer = players[activePlayer];
        switch (playerChoice)
        {
            case 1:
                int direction;
                cout << "Enter a direction to move (0-7)";
                while (!(cin >> direction) || direction < 0 || direction > 7) 
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number between 0 and 7: ";
                }  
                movePlayer(movingPlayer,direction);
                break;
            case 2:
                shoreUp();
                break;
            case 3:
                //get tresure
                break;
            case 4:
                //play card
                break;
            case 5:
                cout << "Special Abilities \n1) Fly (Pilot)\n2) Send Treasure Card (Messenger)\n3) Move Other Player (Navigator)\n4) Return to Main Menu\n";
                int option;
                while (!(cin >> option) || option < 0 || option > 4)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number between 1 and 5: ";
                }
                switch (option)
                {
                    case 1: // fly
                        if(players[activePlayer].getPlayerClass() == 3 && pilotFlight == false)
                        {
                            cout << "Enter a location to fly to: ";
                            int destination;
                            cin >> destination;
                            players[activePlayer].fly(destination);
                            pilotFlight = true;
                        }
                        else
                        {
                            cout << "Cannot fly this turn" << endl;
                        }
                        break;
                    case 2: // send treasure
                        if(players[activePlayer].getPlayerClass() == 6 && players[activePlayer].getHandSize() > 0)
                        {
                            int receivingPlayer;
                            cout << "Enter a player number to give the card to: ";
                            cin >> receivingPlayer;
                            if(receivingPlayer != activePlayer && receivingPlayer > -1 && receivingPlayer < players.size())
                            {
                                int treasureSlot;
                                cout << "Which treasure to give: ";
                                cin >> treasureSlot;
                                if (treasureSlot > -1 && treasureSlot < players[activePlayer].getHandSize())
                                {
                                    transferTreasure(players[activePlayer], players[receivingPlayer], treasureSlot);
                                }
                            }
                            else {cout << "Can't transfer treasure \n";}
                        }
                        break;
                    case 3: // move other player
                        if(players[activePlayer].getPlayerClass() == 4)
                        {
                            int movingPlayer;
                            cout << "Enter the player to move:";
                            cin >> movingPlayer;
                            if(movingPlayer != activePlayer && movingPlayer > -1 && movingPlayer < players.size())
                            {
                                for(int i=0; i<2; i++)
                                {
                                    if(i>0)
                                    {
                                        char moveAgain;
                                        cout << "Move player again (y/n)?";
                                        cin >> moveAgain;
                                        if(moveAgain != 'y' && moveAgain != 'Y')
                                        {
                                            break;
                                        }
                                    }
                                    int direction;
                                    cout << "Enter a direction: ";
                                    cin >> direction;
                                    movePlayer(players[movingPlayer], direction);
                                    printGameState();
                                }
                            }
                        }
                        break;
                    case 4: // return to main menu
                        break;
                }
                break;  
            case 6:
                players[activePlayer].setActions(0);  
        }
    }
    printGameState();
}


void Game::nextPlayer()
{
    activePlayer +=1;
    if (activePlayer > (players.size()-1))
    {activePlayer = 0;}
    players[activePlayer].resetActions();
}