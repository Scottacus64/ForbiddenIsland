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


void Game::drawTreasureCards(int playerSlot)
{
    Player& playerUp = players[playerSlot];
    Card* pDCard;
    Card* pCard;
    if(treasureDeck.deckSize() > 0)
    {
        pCard = treasureDeck.deal();
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
    }
    if(pCard->getTreasureValue() == 7 && gameStarted == false)
    {
        treasureDeck.addCard(pCard);
        treasureDeck.shuffle();
        drawTreasureCards(playerSlot);
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
    }
    else
    {
        pDCard = playerUp.drawCard(pCard);
        if (pDCard != nullptr)
        {
            if(pDCard->getTreasureValue() == 5)
            {
                int cardSlot = players[activePlayer].getTreasureCardSlot(pDCard);
                helo(activePlayer, cardSlot);      
            }
            if(pDCard->getTreasureValue() == 6)
            {
                int cardSlot = players[activePlayer].getTreasureCardSlot(pDCard);
                sandBag(activePlayer, cardSlot);
            }
            treasureDiscard.addCard(pDCard);
        }
    }
}


void Game::transferTreasure(Player& givePlayer, Player& takePlayer, int cardSlot)
{
    Card* pCard = givePlayer.giveTreasureCard(cardSlot);
    takePlayer.getTreasureCard(pCard);
    if(takePlayer.getHandSize() > 5)
    {
        Card* pDCard = takePlayer.discardCard();
        treasureDiscard.addCard(pDCard);
    }
}


void Game::movePlayer(Player& player, int  direction)
{
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
        if(players[activePlayer].getPlayerClass() == 5  || players[mPlayer].getPlayerClass() == 5) //diver
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


bool Game::heloPlayers(int location)
{
    bool canHelo = false;
    vector<Player*>startPlayers;
    for(int i=0; i<players.size(); i++)
    {
        Player& p = players[i];
        if (p.getLocation() == location)
        {
            startPlayers.push_back(&p);
            canHelo = true;
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
    return canHelo;
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
        bool validTarget = true;
        for(int i=0; i<invalidSquares.size(); i++)
        {
            if(target == invalidSquares[i]){validTarget = false;}
        }
        if(target < 0 || target > 35){validTarget = false;}
        if(validTarget == true)
        {
            Card* pCard = islandHand.getCardAt(target);
            if (pCard->getState() == 1)
            {
                pCard->setState(2);
                if(players[activePlayer].getPlayerClass() == 1 && i==0)
                {
                    printGameState();
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


string Game::getIslandCard(int position)
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
    if(pCard->getState() == 0)
    {
        name = "";
    }
    return name;
}


void Game::playerTurn()
{
    pilotFlight = false;
    while(players[activePlayer].getActions() > 0)
    {
        printGameState();
        int playerChoice;
        while (!(cin >> playerChoice) || playerChoice < 1 || playerChoice > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 6: ";
        }
        Player& movingPlayer = players[activePlayer];
        switch (playerChoice)
        {
            case 1: // move
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
            case 2: //shore up
                shoreUp();
                break;
            case 3: //get tresure
                int treasure;
                cout << "Enter a treasure to capture (1-4):";
                cin >> treasure;
                getTreasure(players[activePlayer], treasure);
                break;
            case 4: //play card
                int cardSlot;
                cout << "Enter a card slot to play (0-4):";
                if (!(cin >> cardSlot) || cardSlot < 0 || cardSlot > players[activePlayer].getHandSize()-1) 
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input.\n";
                    break; // now this breaks the switch case
                }
                int cardTValue;
                cardTValue = players[activePlayer].getCardTreasureValue(cardSlot);
                if(cardTValue == 5) // helo
                {
                    helo(activePlayer, cardSlot);
                    players[activePlayer].setActions(-1);
                }
                if (cardTValue == 6) //sandbag
                {
                    sandBag(activePlayer, cardSlot);
                    players[activePlayer].setActions(-1);
                }
                break;
            case 5: //special ability
                cout << "Special Abilities \n"
                "1) Fly (Pilot)\n"
                "2) Send Treasure Card (Messenger)\n"
                "3) Move Other Player (Navigator)\n"
                "4) Return to Main Menu\n";
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
                            players[activePlayer].setActions(-1);
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
                                    players[activePlayer].setActions(-1);
                                }
                            }
                            else {cout << "Can't transfer treasure \n";}
                        }
                        break;
                    case 3: // move other player
                        if(players[activePlayer].getPlayerClass() == 4)
                        {
                            cout << "Enter the player to move:";
                            cin >> mPlayer;
                            if(mPlayer != activePlayer && mPlayer > -1 && mPlayer < players.size())
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
                                    movePlayer(players[mPlayer], direction);
                                    printGameState();
                                }
                                players[activePlayer].setActions(-1);
                            }
                        }
                        break;
                    case 4: // return to main menu
                        break;
                }
                break;  
            case 6: // end turn
                players[activePlayer].setActions(0); 
                break;
            case 7: // give treasure card
                {
                    int rPlayer;
                    int slot;
                    cout << "\nPlayer to give card to: ";
                    cin >> rPlayer;
                    cout << "Slot of card to give: ";
                    cin >> slot;
                    transferTreasure(players[activePlayer], players[rPlayer], slot);
                    players[activePlayer].setActions(-1);
                    break;
                }
        }
    }
    for (int i=0; i<2; i++)
    {
        drawTreasureCards(activePlayer);
        printGameState();
    }
    gameTurn();
}


void Game::sandBag(int player, int cardSlot)
{
    cout << "Enter a location to sandbag: ";
    int location;
    cin >> location;
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
    cout << "Enter a location to helicopter from: ";
    int hLocation;
    cin >> hLocation;
    bool canHelo;
    canHelo = heloPlayers(hLocation);
    if(canHelo == true)
    {
        Card* pCard = players[player].playCardSlot(cardSlot);
        treasureDiscard.addCard(pCard);
    }
}


void Game::gameTurn()
{
    vector<int>floodDrawNumber = {2,2,3,3,3,4,4,5,5};
    int cardsToDraw = floodDrawNumber[waterLevel];
    for(int i=0; i<cardsToDraw; i++)
    {
        char choice;
        int numberOfHeloSandBagCards = 0;
        for(int i=0; i<players.size(); i++)
        {
            int handSize = players[i].getHandSize();
            if(handSize > 0)
            {
                for(int j=0; j<handSize; j++)
                {
                    if(players[i].getCardTreasureValue(j) > 4)
                    {
                        numberOfHeloSandBagCards +=1;
                    }
                }
            }
        }
        bool keepLooping = true;
        if(numberOfHeloSandBagCards > 0)
            {while(keepLooping == true)
            {
                cout << "Do you want to play any helicopter or sand bag cards(y/n)?";
                cin >> choice;
                if(choice == 'y' or choice == 'Y')
                {
                    int pSlot;
                    int cSlot;
                    cout << "Enter a player slot: ";
                    cin >> pSlot;
                    cout << "Enter a card slot: ";
                    cin >> cSlot;
                    if(cSlot < players[pSlot].getHandSize())
                    {
                        Card* pCard = players[pSlot].lookAtCardSlot(cSlot);
                        int cardType = pCard->getTreasureValue();
                        if(cardType == 5)
                        {
                            helo(pSlot, cSlot);
                            numberOfHeloSandBagCards -=1;
                        }
                        if(cardType == 6)
                        {
                            sandBag(pSlot, cSlot);
                            numberOfHeloSandBagCards -=1;
                        }
                    }
                    if(numberOfHeloSandBagCards < 1)
                    {
                        keepLooping = false;
                    }
                }
                else
                {
                    keepLooping = false;
                }
                printGameState();
            }
        }
        int location = flipFlood();
        printGameState();
        bool loss = checkForLoss();
        if(loss == true)
        {
            cout<< "\n ***** GAME OVER *****\n";
            playAgain();
        }
        checkPlayerInWater();
    }
    nextPlayer();
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
                players[i].fly(location);
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
                movePlayer(players[i],direction);
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
    playerTurn();
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