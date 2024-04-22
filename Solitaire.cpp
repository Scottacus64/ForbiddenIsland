#include "Solitaire.h"
#include <array>
#include <vector>
#include <random>

using namespace std;

Solitaire::Solitaire()
{
   //playGame();
} 


Solitaire::~Solitaire()
{

}


void Solitaire::playGame()
{
    solitaireDeck = Deck();                             // makes a deck and shuffles the deck
    solitaireDeck.shuffle();
    clearLinkedList(head);
}


void Solitaire::dealGame()
{
    for (int i=0; i<4; i++) {Aces[i].clearHand();}      // the ace stack is a hand
    for (int i=0; i<7; i++) {cardCol[i].clearHand();}   // the card columns are also hands
    for (int row=0; row<7; row++)
    {
        for (int col=0; col<7; col++)
        {
            if (col == row)                             // if it is the last card in a cardCol
            {
                Card* pCard = solitaireDeck.deal();
                int id = pCard->getID();
                pCard->setFaceUp(true);                 // deal face up
                cardCol[col].addCard(pCard);
            }
            else if (col > row)                         // cols less than row don't get cards
            {
                Card* pCard = solitaireDeck.deal();
                pCard->setFaceUp(false);                // deal face down
                cardCol[col].addCard(pCard);
            }
        }
    }   
    drawPile = Deck(0);                                 // initializes a Deck object called drawPile
    dpSize = 0;
    win = false;
    moves = 0;
    printField();
}


Hand* Solitaire::getColumn(int col)
{
    Hand* pCardColumn = &cardCol[col];
    return pCardColumn;
}


Card* Solitaire::getColCardAt(int col, int row)
{
    Card* c = cardCol[col].getCard(row);
    return c;
}


Deck* Solitaire::getDeck()
{
    Deck* p_deck = &solitaireDeck;
    return p_deck;
}


void Solitaire::printField()
 {
    Card* cardArray[7];
    std::cout<<"\n";
    for (int row=0; row<19; row++)
    {
        bool emptyRow = true;
        for (int col=0; col<7; col++)
        {
            if (row < cardCol[col].getSize())
            {
                Card* pCard = cardCol[col].getCard(row);
                cardArray[col]= pCard;
                emptyRow = false;
            }
            else
            {
                cardArray[col] = nullptr;
            }
        }
        if (emptyRow == false)
        {
            for (int i=0; i<7; i++)
            {
                Card* pCard = cardArray[i];
                if (pCard)
                {
                   // pCard->flipFaceUp();
                    pCard->printCard();
                }
                cout << "\t";
            }
            cout << "\n";
        }
    }
    saveGameState();
 }


Card* Solitaire::getTopDrawPileCard()
{
    // cout << "GTCDP deck = " << &drawPile << endl;;
    Card* pCard = drawPile.getTopDeckCard();
    drawPile.printTopCard();
    // cout << "***** GTCDP Card Address = " << pCard << "\n\n";
    int id = pCard->getID();
    // cout << "****** GTDPC id = " << id << endl;;
    return pCard;
}


Deck* Solitaire::getDrawPile()
{
    Deck* p_drawPile = &drawPile;
    return p_drawPile;
}


int Solitaire::getDrawPileSize()
{
    int dpSize = drawPile.cardsLeft();
    return dpSize;
}


int Solitaire::getDeckSize()
{
    int size = solitaireDeck.cardsLeft();
    return size;
}


Card* Solitaire::getDrawPileAt(int slot)
{
    Card* m_pc = drawPile.getDeckCardAt(slot);
    return m_pc;
}


int Solitaire::getColumnSize(int col)
{
    int size = cardCol[col].getSize();
    return size;
}



int Solitaire::getTopAce(int suit)
{
    int id = Aces[suit].getLastCardID();
    return id;
}


Hand* Solitaire::getAceStack(int suit)
{
    Hand* aceStack = &Aces[suit];
    return aceStack;
}


bool Solitaire::getWin()
{
    return win;
}


int Solitaire::cycleDeck()
{
    if (solitaireDeck.cardsLeft() == 0 && getDrawPileSize() > 0){reuseDeck();}  // puts the cards back into the solitaire deck
    int cardsDelt = 0;
    for (int i=0; i<3; i++)
    {
        if (solitaireDeck.cardsLeft() > 0)
        {
            Card* pCard = solitaireDeck.deal();          // get and remove the Card object from the solitaireDeck
            pCard->flipFaceUp();                         // set it's face up bool to true
            int id = pCard->getID();
            drawPile.addCard(pCard);                    // add the Card object to the deck drawPile
            pCard->printCard();
            dpSize = getDrawPileSize();
            cardsDelt ++;
        }
        Card* pCard = drawPile.getTopDeckCard();
        int id = pCard->getID();
    }
    
    cout << "\033[0m" << "\ntop card: ";
        
    int remaining = solitaireDeck.cardsLeft();
    cout << " Cycle deck cards remaining = " << remaining << "\n\n";
    if (remaining == 0){bool win = checkForWin();}
    moves ++;
    return cardsDelt;                               // this let's the calling function know how many cards were delt
}



/*******************************************************************************************
 *          This is the setion that looks over the ID number and suit of a card sent to it         
 *          to check if that card can be played on either a column or the aces above.  
 *          It will also move that card to the highest priority location       
********************************************************************************************/
bool Solitaire::checkCanMove(Card* pCard, int col, int row, bool lastCard, bool lastUnflippedCard)
{
    bool cardRed;
    bool columnRed;
    bool sameCardClicked = false;
    bool canMove = false;
    char suits[4] = {'C', 'S', 'H', 'D'};
    int moveSize;
    bool aceMatch = false;

    possibleMoves.clear();
    cout << "CheckCanMove's Card address is: " << pCard << endl;;
    bool fu = pCard->getFaceUp();
    cout << "the card in checkCanMove's facuUp value is: "  << fu << endl;;

    if (pCard->getFaceUp() == true)                   // if the card is not flipped up, disregard it
    {
        int id = pCard->getID();                      // from the card's ID, face value and suit
        cout << "CheckPosMoves id = " << id << endl;;

        /********  check if the same card was clicked again  *******/
        if (id == lastCardClicked)                  // if the same card is clicked again
        {
            sameCardClicked = true;                 // if the same card is clicked again      
            cardCycle ++;                           // increment the counter
            moveSize = possibleMoves.size();
            if (cardCycle >= moveSize)              // if the counter has reached the possible spots for the card
            {
                cardCycle = 0;                      // reset the counter to zero
                aceFlag = false;                    // if the card can go to an aceStack reset so it can go back up there
            }
        }
        else 
        {
            cardCycle = 0;                          // otherwise a new card has been clicked so reset the counter
            aceFlag = false;                        // reset the ace flag as well
        }           

        possibleMoves.clear();                      // clear out possible moves now that we've passed the checks
        lastCardClicked = id;                       // set an id to check next click to see if the same card is clicked

        int cardValue = pCard->getFaceValue();         
        char suit = pCard->getSuit();

        cout << "In CheckPossMoves CardValue = " << cardValue << " suit = " << suit << endl;;

        /************ check each of the four aceStacks at the top to see if the card can move here ***********/
        for (int j=0; j<4; j++)                     // check if a card can play on an ace stack at the top of the table
        {
            // if the array is not empty, a card has not been moved to aces and it is the last card in the cardCol
            cout << "aceStack" << j << " size = " << Aces[j].getSize()<< " LC = " << lastCard << " AM = " << aceMatch << endl;;
            if (Aces[j].getSize() > 0 && lastCard == true && aceMatch == false)  
            {
                int aceID = Aces[j].getLastCardID();             // get the id of the last card in the Aces stack
                int aceStackValue = aceID%13;
                char aceSuit = Aces[j].getLastCardSuit();        // as well as its suit
                int cardID = id%13;
                if (cardID == 0) {cardID = 13;}
                cout << "aceStack last card= " << aceStackValue << " incoming card = " << cardID << endl;;
                if (cardID == aceStackValue+1 && suit == aceSuit)      // if the clicked card's id is one more than the last one in the ace stack
                {
                    possibleMoves.push_back((j+1)*10);          // set it as a possible move 
                    aceMatch = true;
                }
            }
            else                                   // if the array is empty, the card is an ace and matches the slot suit
            {
                if (id % 13 == 1 && suit == suits[j] && aceMatch == false)  // if it is an Ace card in play
                {
                    possibleMoves.push_back((j+1)*10);  // use a multiple of ten to show this special situation  
                    aceMatch = true;                                        
                }
            }
        }  

        /********** Check each of the seven card columns to see if the card can move to one of these ***********/
        for (int i=0; i<7; i++)                         // go through each cardCol
        {
            int colID = cardCol[i].getLastCardID();     // get the last card in the column's ID
            int colVal = colID % 13;
            if (colVal == 0) {colVal = 13;}             // set Kings to 13
            char colSuit = cardCol[i].getLastCardSuit();

            if (colID < 27) {columnRed = false;}        // set cardCol card as red or black
            else {columnRed = true;}

            if (id < 27) {cardRed = false;}             // set the clicked card as red or black
            else {cardRed = true;}

            // if the card is one less in value than the last card in a cardCol and the color of the card is opposite

            if (colVal == cardValue+1 && columnRed != cardRed)    
            {
                possibleMoves.push_back(i);             // set this as a possible move
            }
        }

        /*********  check if the card is a king and there is an empty cardColumn  **********/
        for (int i=0; i<7; i++)
        {
            int size =cardCol[i].getSize();
            if (size == 0 && cardValue == 13)
            {
                possibleMoves.push_back(i);
            }
        }
    }

    moveSize = possibleMoves.size();
    if (moveSize>0) {canMove=true;}
    cout << "Possible moves: " ;
    for (int i=0; i<moveSize; i++)
    {
        cout << possibleMoves[i] << "/";
    }
    cout << "\n";

    /*****************  determine where the card will move to if it is possible for the card to move ****************/
    for (int slot=0; slot<moveSize; slot++)     // go through each possible move
    {
        cout << "slot = " << slot << " move size = " << moveSize << " CardCycle = " << cardCycle << endl;;
        int destination = possibleMoves[slot];
        cout << "destination = " << destination << endl;;
        bool aceMatch = false;                      // this prevents the ace from being added more than once

        /********* this checks if a card can move to an ace stack which is the highest priority move *********/
        if (destination > 9 && aceFlag == false)    
        {
            int suit = (destination/10)-1;          // calculate the suit from the x10 value     
            aceStackMove(col, row, suit, pCard, lastCard);    // move the card to the ace stack   
            aceMatch = true;                        
        }
        /********* this checks for moving to a cardColumn **********/
        else
        {
            bool moved = false;   // this prevents more than one card to be added if more than one possible column
            if (aceMatch == false and moved == false)   // need to skip if the card has moved to an aceStack
            {
                if (slot == cardCycle)
                {
                    moveToColumn(destination, col, row, lastUnflippedCard); 
                    moved = true;
                }
            }             
        }
    }
    bool win = checkForWin();
    return canMove;
}



/******************  this will move the card to a column in the play area  *******************/
void Solitaire::moveToColumn(int destinationCol, int col, int slot, bool lastUnflippedCard)
{
    cout << "In MoveToColumn\n";
    if (col < 100)                              // this means that it came from the play area and not aces or draw pile
    {
        int colSize = cardCol[col].getSize();
        for (int i=slot; i<colSize; i++)                    // take each card from the selected card to the end
        {
            Card* pCard = cardCol[col].removeCard(slot);                  // remove it from the sorce and..
            cardCol[destinationCol].addCard(pCard);                      // add it to the destination
        }
        int colLength = cardCol[col].getSize();
        if (colLength > 0)                                          // if there are still cards in the col, flip one
        {
            Card* t = cardCol[col].getCard(colLength-1);
            if (lastUnflippedCard == true) {t->flipFaceUp();}
        }
    }
    else 
    {
        if (slot >99)           // this is coming from the drawPile
    {
        playFromDrawPile(destinationCol);
        
    }
        else                    // this is coming from the aces
        {
            playFromAces(destinationCol, slot);
        }
    }
    moves ++;
}


void Solitaire::playFromDrawPile(int col)
 { 
    bool canMove;
    Card* pCard = drawPile.deal();
    cardCol[col].addCard(pCard);
    int dpSize = drawPile.cardsLeft();
}


void Solitaire::playFromAces(int col, int suit)
{
    int size = Aces[suit].getSize();
    Card* pCard = Aces[suit].removeCard(size-1);
    cardCol[col].addCard(pCard);  
    moves ++;
}


void Solitaire::aceStackMove(int col, int row, int suit, Card* pCard, bool lastCard)
{
    Card* pTmpCard;
    if (col<100)
    {      
        pTmpCard = removeForAce(col, row);                  // remove the card from the cardCol    
    }
    else
    {
        pTmpCard = drawPile.deal();
    }
    Aces[suit].addCard(pTmpCard);                           // move the card to the Ace stack
    aceFlag = true;

    win = true;                                             // this checks for the win condition
    for (int i=0; i<4; i++)                                   
    {
        int aceSize = Aces[i].getSize();
        if (aceSize < 13) {win = false;}
    }
    moves ++;
}


Card* Solitaire::removeColCard(int col, int row, bool lastCard)
{
    Card* pCard;
    int length = getColumnSize(col);
    for (int slot=row; slot<length; slot++)             // remove each card from the column
    {
        pCard = cardCol[col].removeCard(slot);
    }

    if (row > 0 && lastCard == true)
    {
        Card* pTmpCard = cardCol[col].getCard(row-1);
        pTmpCard->flipFaceUp();
    }
    return pCard;
 }


 Card* Solitaire::removeForAce(int col,int row)
 {
    Card* pCard = cardCol[col].removeCard(row);
    if (row > 0)
    {
        Card* pTmpCard = cardCol[col].getCard(row-1);
        pTmpCard->flipFaceUp();
    }
    return pCard;
 }


 void Solitaire::reuseDeck()
 {
    int pileSize = drawPile.cardsLeft();
    for (int i=0; i<pileSize; i++)
    {
        Card* pCard = drawPile.deal();
        dpSize --;
        pCard->setFaceUp(false);
        solitaireDeck.addCard(pCard);
    }
 }

 bool Solitaire::checkForWin()
 {
    bool autoFinish = true;
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<cardCol[i].getSize(); j++)
        {
            Card* pTestCard = cardCol[i].getCard(j);
            if (pTestCard->getFaceUp() == false){autoFinish = false;}
        }
    }
    if (getDeckSize() > 0) {autoFinish = false;}
    if (getDrawPileSize() > 0) {autoFinish = false;}
    return autoFinish;
 }

 bool Solitaire::nextCard()
 {
    int totalCards = 0;
    int lowestColumn;
    for (int i=0; i<7; i++) {totalCards = totalCards + cardCol[i].getSize();}
    if (totalCards > 0)
    {
        int cardValues[7];
        for (int i=0; i<7; i++){cardValues[i] = cardCol[i].getLastCardValue();}
        int lowestValue = 20;
        for (int i=0; i<7; i++)
        {
            if (cardValues[i] > 0)
            {
                if (cardValues[i] < lowestValue)
                {
                    lowestValue = cardValues[i];
                    lowestColumn = i;
                } 
            }
        }
        int row = cardCol[lowestColumn].getSize() - 1;
        Card* pCard = cardCol[lowestColumn].removeCard(row); 
        int ID = pCard->getID();
        int suit = ID/14;
        Aces[suit].addCard(pCard); 
        return false;
    }
    else
    {return true;}
 
 }


int Solitaire::getMoves()
{
    return moves;
}

/************************************************************/
void Solitaire::printNode(Solitaire::GameNode* node)
{
    for (int i=0; i<52;  i++)
    {
        Card* pCard = node->gameState[i].pCard;
        int cSlot = node->gameState[i].slot;
        if (cSlot > 210)
            {
            std::cout << "Pointer: " << pCard << " *****************\n";
            std::cout << "Card " << i << ":" << pCard->getID() << " " << pCard->getPrintValue() << pCard->getSuit() <<"\n";
            std::cout << "Draw Pile Slot: " << (cSlot-211)%26 << "\n";
            }
    }
}

void Solitaire::saveGameState()
{
    Solitaire::GameNode* newNode = new Solitaire::GameNode; // make a GameNode* called newNode
    int cardSlot;
    cardSlot = 0;
    for (int i=0; i<4; i++)                             // for each of the Ace stacks
    {
        for (int j=0; j<13; j++)    
        {
            int aceLen = Aces[i].getSize();             // see if there are any cards there
            if (j < aceLen)
            {
                Card* pCard = Aces[i].getCard(j);       // get the Card objecrt
                if (pCard)
                {
                    CardState sCard;
                    sCard.pCard = pCard;                // set the struct's pointer, faceUp and locations
                    sCard.faceUp = pCard->getFaceUp();
                    sCard.slot = j + (i * 13);
                    newNode->gameState[cardSlot] = sCard;   // put the struct in the node being built
                    cardSlot ++;
                }
            }
        }
    }

    for (int i=0; i<7; i++)                             // next check each card column 
    {
        for (int j=0; j<19; j++)
        {
            int colLen = cardCol[i].getSize();
            if (j < colLen)                             // if the column has a card in this slot then...
            {
                Card* pCard = cardCol[i].getCard(j);
                if (pCard)
                {
                    CardState sCard;
                    sCard.pCard = pCard;                
                    sCard.faceUp = pCard->getFaceUp();
                    sCard.slot = 52 + j + (i*19);       
                    newNode->gameState[cardSlot] = sCard;
                    cardSlot ++;
                }
            }
        }
    }  

    int sdLen = solitaireDeck.cardsLeft();              // check the Deal Deck called solitaireDeck
    for (int i=0; i<26; i++)
    {
        if (i < sdLen)
        {
            Card* pCard = solitaireDeck.getDeckCardAt(i);
            if (pCard)
            {
                CardState sCard;
                sCard.pCard = pCard;
                sCard.faceUp = pCard->getFaceUp();
                sCard.slot = 185 + i;
                newNode->gameState[cardSlot] = sCard;
                cardSlot ++;
            }
        }
    }

    int dLen = drawPile.cardsLeft();                    // last check the Draw Pile
    for (int i=0; i<26; i++)
    {
        if (i < dLen)
        {
            Card* pCard = drawPile.getDeckCardAt(i);
            if (pCard)
            {
                CardState sCard;
                sCard.pCard = pCard;
                sCard.faceUp = pCard->getFaceUp();
                sCard.slot = 211 + i;
                newNode->gameState[cardSlot] = sCard;
                cardSlot ++;
            }
        }
    }
    newNode->next = head;                           // assign the current head to newNode's next value
    head = newNode;                                 // make newNode the Head
}


void Solitaire::loadGameState()
{
    if (head)                                           // Check if there's at least one saved game state.
    {  
        Solitaire::GameNode* previousNode = head;       // this makes a node previousNode that points to the current head
        if (head->next != nullptr)                      // make sure you are not at the very first node
        {
            head = head->next;                           // this pushes head back one node in the list
            for (int i=0; i<4; i++){Aces[i].clearHand();}       // clear the hands and Decks
            for (int i=0; i<7; i++){cardCol[i].clearHand();}
            solitaireDeck.eraseDeck();
            drawPile.eraseDeck();

            for (int i=0; i<52; i++)                            // go through all 52 cards in the game
            {
                Card* pCard = head->gameState[i].pCard;                   // get the Card
                pCard->setFaceUp(head->gameState[i].faceUp);        // its faceUp value
                int cSlot = head->gameState[i].slot;                // where it is located       
                if (cSlot<52)                                       // check if it's in the Aces stacks
                    {
                        Aces[cSlot/13].addCardAt(pCard, cSlot%13);
                    }
                else if (cSlot < 185)                                           // or card calumns
                    {
                        cardCol[(cSlot-52)/19].addCardAt(pCard, (cSlot-52)%19);
                    }
                else if (cSlot < 211)                                           // or Dealing Deck
                    {   
                        solitaireDeck.addCardAt(pCard, (cSlot-185)%26);
                    }
                else                                                            // or Draw Pile
                    {
                        drawPile.addCardAt(pCard, (cSlot-211)%26);
                    }
            }
        }
        else
        {
            std::cout << "No other node\n";
        }
    }    
    else 
    {
        std::cout << "No Head\n";
    }
}


void Solitaire::clearLinkedList(Solitaire::GameNode*& head) 
{
    while (head) 
    {
        GameNode* temp = head;
        head = head->next;
        delete temp;
    }
}

bool Solitaire::checkCanPlay()
{
    // first check the bottom cards of the seven columns
    bool canPlay = false;
    bool test = false;
    Card* pCard;
    std::cout<< "From Column Bottom:\n";
    for (int i=0; i<7; i++)
    {
        int colSize = cardCol[i].getSize();
        if (colSize>0)
        {
            bool nextDown;
            pCard = cardCol[i].getLastCard();                       // get the last card in the column
            if (colSize>1)
            {
                Card* pNextCard = cardCol[i].getCardAt(colSize-2);  // if 2 or more cards get the one above last
                if (pNextCard->getFaceUp() == false)                // check to see if it is face down
                {nextDown = true;}                                  // if down then the last card is the bottom card
                else
                {nextDown = false;}
            }
            test = testCardMove(pCard, nextDown);
            if (test == true)
            {
                canPlay = true;
                std::cout << pCard->getPrintValue() << pCard->getSuit() << " : ";
            }
        }
    }
    std::cout << "\n";
    std::cout<< "From Column Top:\n";
    // next check the top flipped card in each column
    for (int i=0; i<7; i++)
    {
        if (cardCol[i].getSize()>0)
        {
            pCard = cardCol[i].getFirstFlippedUp();     // find the first card from the top of the column that is face up
            test = testCardMove(pCard, true);
            if (test == true)
            {
                canPlay = true;
                std::cout << pCard->getPrintValue() << pCard->getSuit() << " : ";
            }
        }
    }
    std::cout << "\n";
    // check the top card of the draw pile
    if (drawPile.cardsLeft() > 0)
    {
        pCard = drawPile.getTopDeckCard();
        test = testCardMove(pCard, true);
        if (test == true)
        {
            canPlay = true;
            std::cout << "Draw Pile Top\n" << pCard->getPrintValue() << pCard->getSuit() << "\n";
        }
    }
    // check through all of the remaining play pile
    int deckSize = solitaireDeck.cardsLeft();
    if (deckSize > 0)
    {
        if (deckSize > 2)
            {
                for (int i=deckSize-3; i>-1; i-=3)
                {
                    pCard = solitaireDeck.getDeckCardAt(i);
                    test = testCardMove(pCard, true);
                    if (test == true)
                    {
                        canPlay = true;
                    }
                }
            }
        // check the last card in the draw pile
        pCard = solitaireDeck.getDeckCardAt(0);
        test = testCardMove(pCard, true);
        if (test == true)
        {
            canPlay = true;
            std::cout << pCard->getPrintValue() << pCard->getSuit();
        }
    }
    // check through the play and draw piles each three cards
    Deck tmpDrawPile = drawPile;            //make temp decks
    Deck tmpDeck = solitaireDeck;
    int tdpSize = tmpDrawPile.cardsLeft();
    if (tdpSize > 0)
    {
        for (int i=0; i<tdpSize; i++)
        {
            pCard = tmpDrawPile.deal();     // pop each card off of the top of the temp draw pile
            tmpDeck.addCard(pCard);         // add it to the temp deck
        }
        int tmpDS = tmpDeck.cardsLeft();
        if (tmpDS > 0)
        {
            if (tmpDS > 2)
            {
                for (int i=tmpDS-3; i>-1; i-=3)
                {
                    pCard = tmpDeck.getDeckCardAt(i);
                    bool test = testCardMove(pCard, true);
                    if (test == true)
                    {
                        canPlay = true;
                    }
                }
                // check the last card in the draw pile
                pCard = tmpDeck.getDeckCardAt(0);
                test = testCardMove(pCard, true);
                if (test == true)
                {
                    canPlay = true;
                }
            }
        }
    }
    return canPlay;

}

bool Solitaire::testCardMove(Card* pCard, bool lastCard)
{
    bool canPlay = false;
    bool aceMatch = false;
    bool cardRed;
    bool columnRed;
    bool sameCardClicked = false;
    bool canMove = false;
    char suits[4] = {'C', 'S', 'H', 'D'};
    int moveSize;
    int id = pCard->getID();
    int cardValue = pCard->getFaceValue(); 
    bool topCard = false;
    for (int i=0; i<7; i++) 
    {
        if (pCard == cardCol[i].getCardAt(0)){topCard = true;}  // lets up know that the card has no cards under it
    }       
    char suit = pCard->getSuit();
    /************ check each of the four aceStacks at the top to see if the card can move here ***********/
    for (int j=0; j<4; j++)                     // check if a card can play on an ace stack at the top of the table
    {
        // if the array is not empty, a card has not been moved to aces and it is the last card in the cardCol
        if (Aces[j].getSize() > 0 && lastCard == true && aceMatch == false)  
        {
            int aceID = Aces[j].getLastCardID();             // get the id of the last card in the Aces stack
            int aceStackValue = aceID%13;
            char aceSuit = Aces[j].getLastCardSuit();        // as well as its suit
            int cardID = id%13;
            if (cardID == 0) {cardID = 13;}
            if (cardID == aceStackValue+1 && suit == aceSuit)      // if the clicked card's id is one more than the last one in the ace stack
            {
                canPlay = true;          // set it as a possible move 
                aceMatch = true;
            }
        }
        else                                   // if the array is empty, the card is an ace and matches the slot suit
        {
            if (id % 13 == 1 && suit == suits[j] && aceMatch == false)  // if it is an Ace card in play
            {
                canPlay = true;  // use a multiple of ten to show this special situation  
                aceMatch = true;                                        
            }
        }
    }  

    /********** Check each of the seven card columns to see if the card can move to one of these ***********/
    for (int i=0; i<7; i++)                         // go through each cardCol
    {
        int colID = cardCol[i].getLastCardID();     // get the last card in the column's ID
        int colVal = colID % 13;
        if (colVal == 0) {colVal = 13;}             // set Kings to 13
        char colSuit = cardCol[i].getLastCardSuit();

        if (colID < 27) {columnRed = false;}        // set cardCol card as red or black
        else {columnRed = true;}

        if (id < 27) {cardRed = false;}             // set the clicked card as red or black
        else {cardRed = true;}

        // if the card is one less in value than the last card in a cardCol and the color of the card is opposite
        if (colVal == cardValue+1 && columnRed != cardRed && lastCard == true)    
        {
            canPlay = true;             // set this as a possible move
        }
    }

    /*********  check if the card is a king and there is an empty cardColumn  **********/
    for (int i=0; i<7; i++)
    {
        int size =cardCol[i].getSize();
        if (size == 0 && cardValue == 13 && topCard == false)
        {
            canPlay = true;
        }
    } 
    return canPlay;
}


void Solitaire::makeWinnableDeck()
{
    solitaireDeck = Deck();                             // makes a deck                
    solitaireDeck.shuffle();                            // shuffles
    clearLinkedList(head);                              // sets up the linked list for undoing moves
    buildColumns();                                     // starts up the process of making a deck that can win
}

// Step 1 make four columns
void Solitaire::buildColumns()
{
    for (int i=0; i<4; i++) {Aces[i].clearHand();}      // clear the ace stacks
    for (int i=0; i<7; i++) {cardCol[i].clearHand();}   // clear the columns
    for (int i=13; i>0; i--)                            // go from king to ace
    {
        int colNumber = 3;                              // place in columns 3-6
        int deckSize = solitaireDeck.cardsLeft();
        for (int j=0; j<4; j++)
        {
            for (int k=0; k<deckSize; k++)
            {
                Card* cCard = solitaireDeck.getDeckCardAt(k);
                int cValue = cCard->getFaceValue();
                if (cValue == i)
                {
                    if (i==13)                                  // check if a King
                    {
                        Card* pCard = solitaireDeck.dealCardAt(k);
                        pCard->flipFaceUp();                    // flip the card over to show its top side
                        cardCol[colNumber].addCard(pCard);      // add it to the card column
                        colNumber ++;
                        deckSize --;
                    }
                    else                                        // card is Q-A 
                    {
                        Card* pCard = solitaireDeck.dealCardAt(k);
                        deckSize --;
                        pCard->flipCard();                      // flip the card over to show its top side
                        for (int m=3; m<7; m++)                 // check columns 3-6
                        {   
                            int pRedBlack = pCard->getBlackRed();   // get the red/black of the moving card
                            int pValue = pCard->getFaceValue();     // get the face value of the moving card
                            Card* tCard = cardCol[m].getLastCard(); // get the last card in the column
                            int tRedBlack = tCard->getBlackRed();   // check that card's red/black
                            int tValue = tCard->getFaceValue();     // and face value
                            if (pRedBlack != tRedBlack && tValue > pValue)  // check to see if card can play
                            {
                                cardCol[m].addCard(pCard);          // add the card to the column
                                break;                              // and exit the loop
                            }

                        }
                    }
                }
            }
        }
    }
    buildAceStacks(); // once the four columns K-A of alternating red and black cards have been built move on to ace stacks
}


// Step 2 move a randfom number of cards from the columns to the ace stacks
void Solitaire::buildAceStacks()
{
    std::random_device rd;                                      // Seed the random number generator
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> aceMoves(22, 28);        // Define the range [22, 28]
    std::uniform_int_distribution<int> checkCol(3, 6);          // Define the range [3, 6]
    int numberToAces = aceMoves(gen);                           // Generate a random number ace moves from columns
    for (int i=0; i<numberToAces; i++)  
    {
        bool canMove = false;
        while (canMove == false)
        {
            int col = checkCol(gen);                                    // get a random column
            int cCardFaceValue = cardCol[col].getLastCardValue();
            int cCardId = cardCol[col].getLastCardID();
            for (int j=0; j<4; j++)                                     // go through the four columns
            {
                if (cCardFaceValue > 1)                                 // if the card is not an ace
                {
                    if (cCardId-1 == getTopAce(j))                      // if the moving card's ID is one greater than the acestack top card
                    {  
                        int pos = cardCol[col].getSize() - 1;
                        Card* pCard = cardCol[col].removeCard(pos);
                        Aces[j].addCard(pCard);                         // move the card to the Ace stack
                        canMove = true;                                 // set to break out of the while loop
                        break;
                    }
                }
                else                                                    // if it is an ace, same as above
                {
                    int pos = cardCol[col].getSize() - 1;
                    Card* pCard = cardCol[col].removeCard(pos);
                    int suit = pCard->getSuitInt();
                    Aces[suit].addCard(pCard);                          
                    canMove = true;
                    break;
                }
            }

        }

    }
    solitaireDeck.eraseDeck();      // are these needed?
    drawPile.eraseDeck();
    finishDeck();
}


// Step 3 move the columns and ace stacks to a starting position
void Solitaire::finishDeck()
{
    int  aceCards;
    bool acesGone = false;
    bool dpFull = false;
    bool allDone = false;
    bool lastFour[7];
    int  cycleCount = 0;
    std::random_device rd;                                          
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randHundred(1, 100);     // generate generic 1 to 100 for percentage moves 
    for (int i=0; i<7; i++)
    {
        lastFour[i] = false;    // looks at the last four columns (3-6) and tries to make sure they are fully cycled 90% of the time
        int rndFour= randHundred(gen);
        if (rndFour<10 && i<4){lastFour[i]= true;}     // 10% chance lastFour[0-3] will start out as true
    }

    while (allDone == false)
    {
        int activeColumns = 0;      // this is the number of column that still can take face down cards
        int aceCards = 0;           // this is the number of cards in ace stacks
        int colCards = 0;           // this is the number of cards in columns that are face down
        int colActive[7];           // this is a column that a card can legally be moved to 
        cycleCount ++;              // used to make column play more likely than ace play for a certain number of cycles
        // Find out how many columns can be played to and how many cards are available to play
        for (int i=0; i<7; i++)
        {
            int firstUp = cardCol[i].getFirstFlippedUpPosition();   // this is the location in the col of the first face up card (0 = first slot)
            int colSize = cardCol[i].getSize();
            bool allDown = cardCol[i].allFaceDown();
            if (allDown && ((colSize > 0 && i > colSize-1)||colSize == 0)) {activeColumns = activeColumns+1;}
            // this flips the last card in a column down if there are no other face up cards and the column is not complete
            if (colSize-firstUp == 1 && firstUp<i && firstUp>-1) // colSize-firstUp will show if last card, firstUp<i will show noncompleted column                  
            {
                if (i<3 || lastFour[i]== true)     // flip down if col 0-2 or 3-6 already cleared
                {
                    Card* pCard = cardCol[i].getCardAt(firstUp);
                    pCard->setFaceUp(false);
                }
            }
            // first up will be -1 if there are no face up cards, needed because position 0 is the first slot
            if (firstUp>-1)
            {
                if (colSize-firstUp == 1 && i==firstUp)
                {
                }
                else
                {
                    colCards = colCards + colSize - firstUp;
                }
            }
            else
            {colCards = colCards + colSize;}
        }
        if (cycleCount < 20){colCards = colCards + 30;}         // for the first 20 cycles pad colCards to make that more likely to happen
        // Find out how many cards in the ace stacks
        for (int i=0; i<4; i++)
        {
            int aceSize = Aces[i].getSize();
            aceCards = aceCards + aceSize;
        }
        int allPlays = aceCards + colCards;                         // sum up how many cards still need a home
        std::random_device rd;                                          
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> aceOrCol(0, allPlays);   // generate a number for choosing aces or columns     
        std::uniform_int_distribution<int> aceSelect(0, 3);         // generate a number for choosing an ace stack  
        std::uniform_int_distribution<int> randHundred(1, 100);     // generate a generic 1 to 100 for percentage moves 

        int rndAceCol = aceOrCol(gen);                              // number for determining ace or column move    
        Card* mCard;  

        /******** Working with the Aces *********/                            
        if (rndAceCol < aceCards)                               // if that number falls in the ace moves
        {
            int colOrDeck = randHundred(gen);
            if (colOrDeck < 90 && activeColumns > 0)            // 90% chance to move to a column if there is a col that can take a card
            {
                bool play = false;
                while (play == false)                           // while there is no play
                {
                    int aceChoice = aceSelect(gen);
                    int aceSize = Aces[aceChoice].getSize();    
                    if (aceSize > 0)                            // if the size of the ace stack is greater than zero
                    {
                        mCard = Aces[aceChoice].removeCard(aceSize-1);  // get a card from the ace stack
                        mCard->setFaceUp(false);
                        moveToActiveColumn(mCard);              // set the control bool to true to exit the while
                        play = true;
                    }
                }
            }
            else                                                // 10% chance to move to the draw pile
            {
                bool play = false;                              // note play will always go to true since there will always be ace cards for this branch to be selected
                if (drawPileFull() == false)               
                {
                    while (play == false)
                    {
                        int aceChoice = aceSelect(gen);
                        int aceSize = Aces[aceChoice].getSize();
                        if (aceSize > 0)
                        {
                            mCard = Aces[aceChoice].removeCard(aceSize-1);
                            moveToDrawPile(mCard);
                            play = true;
                        }
                    }
                }
            }
        }
        /******** Working with columns *********/
        else                                                    
        {
            if (activeColumns > 0)                              // if there are open columns to play to
            {   
                int multOrSingle = randHundred(gen);
                if (multOrSingle < 40)                          // 40% chance to move a single card
                {
                    int lowestCol = findSmallestColumn();
                    int colSize = cardCol[lowestCol].getSize();
                    if (colSize > 1 || (lowestCol>2 && lastFour[lowestCol]==false)) // col of 2 face up or last 4 col have not been fully cycled to clear kings
                    {
                        mCard = cardCol[lowestCol].removeCard(colSize-1);   // get the card to be moved
                        if (lowestCol>2 && colSize-1==0)          //first 4 col need to fully turn over
                        {
                            lastFour[lowestCol] = true;
                        }
                        int dpOrCol = randHundred(gen);
                        if (dpOrCol < 80)                           // 80% chance to move to a Column
                        {
                            moveToActiveColumn(mCard);              // move the card to a Column    
                        }
                        else                                        // 20% chance to move to Draw Pile
                        {
                            if (drawPileFull() == false)
                                {moveToDrawPile(mCard);}            // move the card to the Draw Pile
                        }
                    }
                }
                /************* Move Multiple Cards ***************/
                else                                            // 40% chance to move multiple cards
                {                   
                    for (int i=0; i<7; i++){colActive[i] = 10;} // set them all to invalid values
                    int counter = 0;                            // counts the number of active columns
                    for (int i=0; i<7; i++)                     // go through the columns
                    {
                        int colSize = cardCol[i].getSize();
                        if (cardCol[i].allFaceDown() == true && ((colSize > 0 && i > colSize-1)||colSize == 0)) // if the column is active
                        {
                            colActive[counter] = i;
                            counter ++; // Note counter increments after the colActive is set so counter is 1 greater than the number of cols
                        }
                    }
                    if (counter > 0)
                    {
                        std::uniform_int_distribution<int> randCol(0, counter-1);   // generate a number for choosing which col to move to 
                        int destination = randCol(gen);                             // generate a number for a column
                        int destinationCol = colActive[destination];                // find the actual column to transfer to
                        /****** find the columns with more than one face up card ******/
                        int colCardsUp[7];
                        int upCards;
                        for (int i=0; i<7; i++){colCardsUp[i] = 0;}
                        for (int i=0; i<7; i++)                 
                        {
                            int firstUpSlot = cardCol[i].getFirstFlippedUpPosition();
                            if (firstUpSlot > -1)
                            {upCards = cardCol[i].getSize() - firstUpSlot;}
                            else
                            {upCards = 0;}
                            if (upCards > 1)
                            {
                                colCardsUp[i] = upCards;    // add the number of face up cards to the array
                            }
                        }
                        int largestCol = 0;                 // this will be the slot of the column with the most face up cards
                        int firstCol = 0;
                        bool colFound = false;
                        for (int i=0; i<7; i++)
                        {
                            if (colCardsUp[i]>0 && firstCol==0)  // if there are no lowest columns yet then get the first one
                            {
                                largestCol = i;
                                firstCol = 1;
                                colFound = true;
                            }
                            if (i>0 && firstCol == 1 && colCardsUp[i]>largestCol) // if we are past the zero slot and have at least
                            {                                                     // one largest column then see if the next one is 
                                largestCol = i;                                   // larger to find the biggest one
                            }
                        } 
                        if (colFound == true)           // there is a largest column
                        {
                            std::uniform_int_distribution<int> randColSplit(1, (colCardsUp[largestCol]-1));   
                            int splitPoint = randColSplit(gen);     // generate a random point to split the up cards for moving them
                            int colSize = cardCol[largestCol].getSize();
                            splitPoint = colSize-splitPoint;   
                            std::cout << "SplitPoint = " << splitPoint << " Col to move from = " << largestCol << " Destination = " << destinationCol << "\n";   
                            //NOTE because the col size is decreasing with each iteration, the point of removal Always is splitPoint
                            for (int i= splitPoint; i<colSize; i++)                    // iterate through the number of cards
                            {
                                Card* pCard = cardCol[largestCol].removeCard(splitPoint);    // remove it from the sorce and..
                                cardCol[destinationCol].addCard(pCard);                      // add it to the destination
                            }
                            int colLength = cardCol[largestCol].getSize();
                            if (colLength==0 && largestCol>2)
                            {
                                lastFour[largestCol]= true;
                            }
                        }
                    }
                }
            }
            /******* There are no active Columns so move a single card to the draw pile from a column *******/
            else                                                
            {
                std::cout << "In no active cols so just move a card to the draw pile \n";
                bool canMove = false;
                int counter = 0;
                while (canMove == false)
                {
                    int possMove[7];
                    int mCounter = 0;
                    for (int i=0; i<7; i++)
                    {
                        int colSize = cardCol[i].getSize();
                        int firstUp = cardCol[i].getFirstFlippedUpPosition();
                        if (colSize-firstUp!=1 && colSize-1!=firstUp || (i>2 && lastFour[i]==false))
                        {
                            possMove[mCounter]=i;
                            mCounter ++;
                        }
                    }
                    if (mCounter > 0)
                    {
                        std::uniform_int_distribution<int> randCol(0, mCounter-1);   // generate a number for choosing which col to move to 
                        int colToUse = randCol(gen); 
                        int takeFrom = possMove[colToUse];
                        int colSize = cardCol[takeFrom].getSize();   
                        int firstUp = cardCol[takeFrom].getFirstFlippedUpPosition();
                        int colFinished = colSize-firstUp;
                        if (drawPileFull() == false) 
                        {
                            mCard = cardCol[takeFrom].removeCard(colSize-1);   // get the card to be moved 
                            if (takeFrom>2 && colSize-1==0)
                            {
                                lastFour[takeFrom] = true;
                            }
                            moveToDrawPile(mCard);
                            canMove = true;
                        }
                        int colTotal = 0;
                        for (int i=0; i<7; i++)
                        {
                            colTotal = cardCol[i].getSize()-cardCol->getFirstFlippedUpPosition()+colTotal;
                        }
                        if (colTotal == 7)
                        {
                            canMove = true;
                        }
                    }
                    counter ++;
                    if (counter>10){canMove = true;}
                }
            }
        }
        aceCards = 0;
        for (int i=0; i<4; i++)
        {
            aceCards = Aces[i].getSize() + aceCards;
        }
        if (aceCards == 0){acesGone = true;}
        int pileCards = drawPile.cardsLeft() + solitaireDeck.cardsLeft();
        if (pileCards > 23){dpFull = true;}
        if (dpFull == true && acesGone == true){allDone=true;}  // we are done building a winnable deck!
        int pileTotal = drawPile.cardsLeft() + solitaireDeck.cardsLeft();
        if(pileTotal > 23 && acesGone == false)    // draw pile is full but deck is not built so start over
        {
            std::cout << "Deck Build Failed, try again!\n";
            allDone = true;
            makeWinnableDeck();
        }
    }
    int pileTotal = drawPile.cardsLeft() + solitaireDeck.cardsLeft();
    if (pileTotal == 24 && acesGone == true)
    {
        while (drawPile.cardsLeft() < 24)
        {cycleDeck();}
        drawPile.printDeck();
        std::cout << "\n";
        int counter = 5;
        for (int i=0; i<7; i++)
        {
            for (int j=6; j>counter; j--)
            {
                Card* pCard = cardCol[j].removeCard(counter+1);
                pCard->setFaceUp(true);
                drawPile.addCard(pCard);
            }
            counter --;
        }
        for (int i=0; i<52; i++)
        {
            Card* pCard = drawPile.dealCardAt(0);
            solitaireDeck.addCard(pCard);
        }
        solitaireDeck.printDeck();
        clearLinkedList(head);
    }
}


bool Solitaire::drawPileFull()
{
    if (drawPile.cardsLeft() + solitaireDeck.cardsLeft() == 24)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Solitaire::moveToActiveColumn(Card* mCard)
{
    std::random_device rd;                                          
    std::mt19937 gen(rd());
    int colActive[7];
    for (int i=0; i<7; i++){colActive[i] = 10;}
    int counter = 0;
    std::cout << mCard->getPrintValue() << mCard->getSuit() << "\n";
    for (int i=0; i<7; i++)                         // go through the columns
    {
        int colSize = cardCol[i].getSize();
        if (cardCol[i].allFaceDown() == true && ((colSize > 0 && i > colSize-1)||colSize == 0)) // if the column is active
        {
            colActive[counter] = i;
            std::cout << "column" << i << " counter = " << counter << "\n";
            counter ++; // Note counter increments after the colActive is set so counter is 1 greater than the number of cols
        }
    }
    std::uniform_int_distribution<int> randCol(0, counter-1);   // generate a number for choosing which col to move to 
    int slot = randCol(gen);                                    // generate a number for a column
    int moveToCol = colActive[slot];
    std::cout << "Random Slot = " << slot << " ColActive = " << moveToCol << "\n";
    cardCol[moveToCol].addCard(mCard);
    if (cardCol[colActive[slot]].getSize() > colActive[slot])
    {
        std::cout << " Face Up ";
        mCard->setFaceUp(true);    // flip the card face up
    }
    else
    {
        std::cout << " Face Down ";
        mCard->setFaceUp(false);
    }
}

void Solitaire::moveToDrawPile(Card* mCard)
{
    std::random_device rd;                                          
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> randFlip(0, 3);
    int upDown = randFlip(gen);             // this method randomly cycles groups of three cards to not stack moved cards on each other
    if (drawPile.cardsLeft() > 3 && upDown > 0)
    {
        for (int i=0; i<upDown; i++)
        {
            cycleDeck();
        }
    }
    mCard->setFaceUp(true);
    drawPile.addCard(mCard);
}

int Solitaire::findSmallestColumn()
{
    // the goal here is to find the column with the least number of face up cards
    int colCardsUp[7];
    int upCardSlot;
    int upCards;
    for (int i=0; i<7; i++){colCardsUp[i] = 0;}
    for (int i=0; i<7; i++)                 // find the columns with more than one face up card
    {
        upCardSlot = cardCol[i].getFirstFlippedUpPosition();
        if (upCardSlot > -1)
        {upCards = cardCol[i].getSize() - upCardSlot;}
        else
        {upCards = 0;}
        if (upCards > 1 || (i<4 && upCards>0))
        {
            colCardsUp[i] = upCards;        // add the number of face up cards to the array
        }
    }
    int lowestCol = 0;                      // this will be the slot of the column with the least face up cards
    int firstCol = 0;
    for (int i=0; i<7; i++)
    {
        if (colCardsUp[i]>0 &&  firstCol == 0)      // if there are no lowest columns yet then get the first one
        {
            lowestCol = i;
            firstCol = 1;
        }
        if (i>0 && firstCol == 1 && colCardsUp[i]< lowestCol && colCardsUp[i]>0) // if we are past the zero slot and have at least
        {                                                     // one lowest column then see if the next one is 
            lowestCol = i;                                    // smaller to find the smallest one
        }
    }
    return lowestCol;
}