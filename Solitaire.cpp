#include "Solitaire.h"
#include <array>
#include <vector>

using namespace std;


Solitaire::Solitaire()
{
    playGame();
} 


Solitaire::~Solitaire()
{

}


void Solitaire::playGame()
{
    solitaireDeck = Deck();
    solitaireDeck.shuffle();
}


void Solitaire::dealGame()
{
    for (int i=0; i<4; i++) {Aces[i].clearHand();}
    for (int i=0; i<7; i++) {cardCol[i].clearHand();}
    for (int row=0; row<7; row++)
    {
        for (int col=0; col<7; col++)
        {
            if (col == row)                         // if it is the last card in a cardCol
            {
                Card c = solitaireDeck.deal();
                int id = c.getID();
                cardCol[col].addCard(c);
            }
            else if (col > row)                     // if it is not the last card in a cardCol
            {
                Card c = solitaireDeck.deal();
                c.flipCard();                       // flip the card over to show its back side
                cardCol[col].addCard(c);
            }
        }
    }   
    drawPile = Deck(0);
    dpSize = 0;
    win = false;
    moves = 0;
    printField();
}


Hand Solitaire::getColumn(int col)
{
    return cardCol[col];
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
    for (int row=0; row<19; row++)
    {
        bool emptyRow = true;
        for (int col=0; col<7; col++)
        {
            if (row < cardCol[col].getSize())
            {
                Card* p_c = cardCol[col].getCard(row);
                cardArray[col]= p_c;
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
                Card* p_c = cardArray[i];
                if (p_c != nullptr){p_c->printCard();}

                cout << "\t";
            }
            cout << "\n";
        }
    }
    gameSaveNode* node = saveGameState();
    printNode(node);
 }


Card* Solitaire::getTopDrawPileCard()
{
    cout << "GTCDP deck = " << &drawPile << endl;;
    Card* p_c = drawPile.getTopDeckCard();
    drawPile.printTopCard();
    cout << "***** GTCDP Card Address = " << &p_c << "\n\n";
    int id = p_c->getID();
    cout << "****** GTDPC id = " << id << endl;;
    return p_c;
}


Deck* Solitaire::getDrawPile()
{
    Deck* p_drawPile = &drawPile;
    return p_drawPile;
}


int Solitaire::getDrawPileSize()
{
    int dpSize = drawPile.cardsLeft();
    std::cout << "getDrawPileSize " << dpSize << "\n";
    return dpSize;
}


int Solitaire::getDrawPileFlipped()
{
    return dpFlipUp;
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
    if (solitaireDeck.cardsLeft() == 0 && getDrawPileSize() > 0){reuseDeck();}
    int cardsDelt = 0;
    cout << "the drawPile address is: " << &drawPile << endl;;
    int dpSize = drawPile.cardsLeft();
    if (dpSize > 0)                             // this flips all draw pile cards face down so that later we can see which
    {                                           // cards are playable (ie face up)
        for (int i=0; i<dpSize; i++) 
        {
            std::cout << "flipping draw pile card\n";
            Card* c = drawPile.getDeckCardAt(i);
            c->setFaceUp(false);
        }
    }
    dpFlipUp = 0;
    for (int i=0; i<3; i++)
    {
        std::cout << "i = " << i;
        if (solitaireDeck.cardsLeft() > 0)
        {
            Card c = solitaireDeck.deal();          // get and remove the Card object from the solitaireDeck
            c.flipFaceUp();                         // set it's face up bool to true
            int id = c.getID();
            Card* p_c = &c;
            cout << "\nthe card delt's id is " << id << endl;;
            cout << "\nthe card delt face up is " << c.getFaceUp() << endl;;
            cout << "the delt cards address is " << &p_c << endl;;
            drawPile.addCard(c);                    // add the Card object to the deck drawPile
            dpFlipUp ++;                            // this shows how many cards in the dp are flipped up and ready to play
            cardsDelt ++;
            c.printCard();
            cout << " ######## draw pile size: " << dpSize << endl;;
        }
        Card* p_c = drawPile.getTopDeckCard();
        int id = p_c->getID();
        cout << "Card Cycle id of top card: " << id << endl;;
    }
    
    cout << "\033[0m" << "\ntop card: ";
    
    drawPile.printTopCard();
    cout << "\033[0m" <<endl;;
        
    int remaining = solitaireDeck.cardsLeft();
    cout << "Cycle deck cards remaining = " << remaining << "\n\n";
    if (remaining == 0){bool win = checkAutoFinish();}
    if (win == true){std::cout << "WIN!!!";}
    moves ++;
    return cardsDelt;                               // this let's the calling function know how many cards were delt
}



/*******************************************************************************************
 *          This is the setion that looks over the ID number and suit of a card sent to it         
 *          to check if that card can be played on either a column or the aces above         
********************************************************************************************/
bool Solitaire::checkCanMove(Card* p_c, int col, int row, bool lastCard, bool lastUnflippedCard)
{
    bool cardRed;
    bool columnRed;
    bool sameCardClicked = false;
    bool canMove = false;
    char suits[4] = {'C', 'S', 'H', 'D'};
    int moveSize;
    bool aceMatch = false;

    possibleMoves.clear();
    cout << "CheckCanMove's Card address is: " << &p_c << endl;;
    bool fu = p_c->getFaceUp();
    cout << "the card in checkCanMove's facuUp value is: "  << fu << endl;;

    if (p_c->getFaceUp() == true)                   // if the card is not flipped up, disregard it
    {
        int id = p_c->getID();                      // from the card's ID, face value and suit
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

        int cardValue = p_c->getFaceValue();         
        char suit = p_c->getSuit();

        cout << "IN CheckPossMoves CardValue = " << cardValue << " suit = " << suit << endl;;

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
            aceStackMove(col, row, suit, p_c, lastCard);    // move the card to the ace stack   
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
    bool win = checkAutoFinish();
    if (win == true){std::cout << "WIN!!!";}
    return canMove;
}



/******************  this will move the card to a column in the play area  *******************/
void Solitaire::moveToColumn(int destinationCol, int col, int slot, bool lastUnflippedCard)
{
    cout << "In MoveToColumn\n";
    if (col < 100)                              // this means that it came from the play area and not aces or draw pile
    {
        int colSize = cardCol[col].getSize();
        cout << "row: " << slot << " col size: " << colSize << " last card: " << lastUnflippedCard << endl;;
        for (int i= slot; i<colSize; i++)                    // take each card from the selected card to the end
        {
            std:: cout << "ROW: " << i << endl;;
            Card c = cardCol[col].removeCard(slot);                  // remove it from the sorce and..
            cardCol[destinationCol].addCard(c);                      // add it to the destination
        }
        int colLength = cardCol[col].getSize();
        cout << "after moving cards colLength = " << colLength << endl;;
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
        cout << "Draw Pile Card\n";
        playFromDrawPile(destinationCol);
        
    }
        else                    // this is coming from the aces
        {
            cout << "aceStack card\n";
            playFromAces(destinationCol, slot);
        }
    }
    moves ++;
}


void Solitaire::playFromDrawPile(int col)
 { 
    bool canMove;
    std::cout << "In playFromDrawPile Draw Pile Size = " << dpFlipUp << "\n"; 
    if(dpFlipUp > 0) 
    {      
        Card c = drawPile.deal();
        cardCol[col].addCard(c);
        dpFlipUp --;
        int dpSize = drawPile.cardsLeft();
        std::cout << "dpFlipUp = " << dpFlipUp << " dpSize = " << dpSize << "\n\n"; 
        if (dpFlipUp == 0 && dpSize >0){flipThreeDP();}
    }
}


void Solitaire::flipThreeDP()
{
    int dpSize = drawPile.cardsLeft();
    if (dpSize > 0)
    std::cout << "In flip three\n\n";
    {
        for (int i=dpSize-1; i>dpSize-4; i--)
        {
            if (i>-1)
            {
                dpFlipUp ++;
                Card* c = drawPile.getDeckCardAt(i);
                c->setFaceUp(true);
            }
        }
    }
}


void Solitaire::playFromAces(int col, int suit)
{
    cout << "In Play from Aces\n";
    int size = Aces[suit].getSize();
    Card c = Aces[suit].removeCard(size-1);
    cardCol[col].addCard(c);  
    moves ++;
}


void Solitaire::aceStackMove(int col, int row, int suit, Card* p_c, bool lastCard)
{
    Card inCard = *p_c;

    cout << "In AceStackMove Suit = " << suit << " Col = " << col << " Row = " << row << endl;;
    cout << "Incomming card is: ";
    inCard.printCard();
    cout << "\n";

    Card c;
    if (col<100)
    {      
        c = removeForAce(col, row);                     // remove the card from the cardCol    
    }
    else
    {
        c = drawPile.deal();
        dpFlipUp --;
        if (dpFlipUp == 0){flipThreeDP();}
    }
    Aces[suit].addCard(c);                              // move the card to the Ace stack
    aceFlag = true;

    win = true;                                         // this checks for the win condition
    for (int i=0; i<4; i++)                                   
    {
        int aceSize = Aces[i].getSize();
        if (aceSize < 13) {win = false;}
    }
    cout << "win = " << win << " Out of AceStackMove\n";
    moves ++;
}



Card Solitaire::removeColCard(int col, int row, bool lastCard)
{
    Card c;
    int length = getColumnSize(col);
        cout << "** RemoveColCard row = " << row << " length = " << length << endl;;
    for (int slot=row; slot<length; slot++)             // remove each card from the column
    {
        c = cardCol[col].removeCard(slot);
    }

    if (row > 0 && lastCard == true)
    {
        Card* p_c = cardCol[col].getCard(row-1);
        p_c->flipFaceUp();
    }
    return c;
 }


 Card Solitaire::removeForAce(int col,int row)
 {
    Card c = cardCol[col].removeCard(row);
    if (row > 0)
    {
        Card* p_c = cardCol[col].getCard(row-1);
        p_c->flipFaceUp();
    }
    return c;
 }


 void Solitaire::reuseDeck()
 {
    int pileSize = drawPile.cardsLeft();
    for (int i=0; i<pileSize; i++)
    {
        Card c = drawPile.deal();
        dpSize --;
        c.setFaceUp(false);
        solitaireDeck.addCard(c);
    }
 }

 bool Solitaire::checkAutoFinish()
 {
    bool autoFinish = true;
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<cardCol[i].getSize(); j++)
        {
            Card* pTestCard = cardCol[i].getCard(j);
            Card testCard = *pTestCard;
            if (testCard.getFaceUp() == false){autoFinish = false;}
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
        Card c = cardCol[lowestColumn].removeCard(row); 
        int ID = c.getID();
        int suit = ID/14;
        Aces[suit].addCard(c); 
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
void Solitaire::printNode(gameSaveNode* node)
{
    std::cout << node << "\n";
    cardState col[7][19];
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<19; j++)
        {
            col[i][j].p_Card = node->sColumn[i][j].p_Card;
            col[i][j].faceUp = node->sColumn[i][j].faceUp;
        } 
    }
    cardState ace[4][13];
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<13; j++)
        {
            ace[i][j] = node->sAces[i][j];
        }
    }
    cardState deck[26];
    for (int i=0; i<26; i++)
    {
        deck[i] = node->sSolitaireDeck[i];
    }
    cardState draw[26];
    for (int i=0; i<26; i++)
    {
        draw[i] = node->sDrawPile[i];
    }
    std::cout <<"Columns";
    for (int i=0; i<7; i++)
    {
        std::cout << "\n";
        for (int j=0; j<19; j++)
        {
            if (col[i][j].p_Card != nullptr)
            {std::cout << col[i][j].p_Card->getID() <<  ":" << col[i][j].faceUp << "\t";}
            else
            {std::cout << "0:0\t";}
        }
    }
    std::cout << "\nAces\n";
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<13; j++)
        {
            if (ace[i][j].p_Card != nullptr)
            {std::cout << ace[i][j].p_Card->getID() << "/" << ace[i][j].faceUp << " ";}
            else 
            {std::cout << "0/0\t";}
        }
        std::cout << "\n";
    }
    std::cout << "Deck\n";
    for (int i=0; i<26; i++)
    {
        if (deck[i].p_Card != nullptr)
        {std::cout << deck[i].p_Card->getID() << ":" << deck[i].faceUp << "\n";}
        else 
        {std::cout <<"0:0\n";}
    }
    std::cout << "Draw Pile\n";
        for (int i=0; i<26; i++)
    {
        if (draw[i].p_Card != nullptr)
        {std::cout << draw[i].p_Card->getID() << ":" << draw[i].faceUp << "\n";}
        else 
        {std::cout << "0:0\n";}
    }

}

/************************************************************/
Solitaire::gameSaveNode* Solitaire::saveGameState() {
    gameSaveNode* newNode = new gameSaveNode;
    cardState dCard;
    dCard.p_Card = nullptr;                     // default card as place holder for slots with no cards in them
    dCard.faceUp = 0;
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<19; j++)
        {
            int colLen = cardCol[i].getSize();
            if (j < colLen)                     // if the column has a card in this slot then...
            {
                Card* pCard = cardCol[i].getCard(j);
                if (pCard != nullptr)
                {
                    Card cCard = *pCard;
                    cardState sCard;
                    sCard.p_Card = pCard;               // pach the data into an sCard struct
                    sCard.faceUp = cCard.getFaceUp();
                    sColumn[i][j] = sCard;              // add the struct to the sColumn
                }
            }
            else {sColumn[i][j] = dCard;}       // use the default card
        }
    }
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<13; j++)
        {
            int aceLen = Aces[i].getSize();
            if (j < aceLen)
            {
                Card* pCard = Aces[i].getCard(j);
                if (pCard != nullptr)
                {
                    Card cCard = *pCard;
                    cardState sCard;
                    sCard.p_Card = pCard;
                    sCard.faceUp = cCard.getFaceUp();
                    sAces[i][j] = sCard;
                }
            }
            else {sAces[i][j] = dCard;}
        }
    }
    int sdLen = solitaireDeck.cardsLeft();
    for (int i=0; i<26; i++)
    {
        if (i < sdLen)
        {
            Card* pCard = solitaireDeck.getDeckCardAt(i);
            if (pCard != nullptr)
            {
                Card cCard = *pCard;
                cardState sCard;
                sCard.p_Card = pCard;
                sCard.faceUp = cCard.getFaceUp();
                sSolitaireDeck[i] = sCard;
            }
        }
        else {sSolitaireDeck[i] = dCard;}
    }
    int dLen = drawPile.cardsLeft();
    for (int i=0; i<26; i++)
    {
        if (i < dLen)
        {
            Card* pCard = drawPile.getDeckCardAt(i);
            if (pCard != nullptr)
            {
                Card cCard = *pCard;
                cardState sCard;
                sCard.p_Card = pCard;
                sCard.faceUp = cCard.getFaceUp();
                sDrawPile[i] = sCard;
            }
        }
        else {sDrawPile[i] = dCard;}
    }
    /* now save all of the above states to the node*/
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<19; j++)
        {
            newNode->sColumn[i][j] = sColumn[i][j]; 
        }
    }
    for (int i=0; i<7; i++)
    {
        for (int j=0; j<19; j++)
        {
            newNode->sAces[i][j] = sAces[i][j]; 
        }
    }
    for (int i = 0; i < 26; ++i) 
    {
        newNode->sSolitaireDeck[i] = sSolitaireDeck[i];
        newNode->sDrawPile[i]= sDrawPile[i];
    }

    newNode->next = head;
    head = newNode;
    std::cout << "**** PRINTING AFTER SAVE ****";
    return newNode;
}

/************************************************************/
void Solitaire::loadGameState() {
    if (head)                               // Check if there's at least one saved game state.
    {  
        gameSaveNode* previousNode = head;  // this makes a node previousNode that points to the current head
        head = head->next;                  // this pushes head back one node in the list
        std::cout << "**** PRINTING AFTER LOAD ****";
        printNode(head);
        for (int i=0; i<7; i++) {cardCol[i].clearHand();}   //clear out the data in the seven card columns
        std::cout << "column undo\n";

        for (int i=0; i<7; i++)
        {
            for (int j=0; j<19; j++)
            {
                if(head->sColumn[i][j].p_Card != nullptr)
                {
                    head->sColumn[i][j].p_Card->setFaceUp(head->sColumn[i][j].faceUp);
                    cardCol[i].addCard(*head->sColumn[i][j].p_Card);
                }
                else
                {j=19;}
            } 
        } 
        for (int i=0; i<4; i++) {Aces[i].clearHand();}
        std::cout << "aces undo\n";
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<13; j++)
            {
                if(head->sAces[i][j].p_Card != nullptr)
                {
                    head->sAces[i][j].p_Card->setFaceUp(head->sAces[i][j].faceUp);
                    Aces[i].addCard(*head->sAces[i][j].p_Card);
                }
                else
                {j=13;}
            }

        } 
        solitaireDeck.eraseDeck();
        std::cout << "main deck undo\n";
        for (int i=0; i<26; i++)
        {
            if(head->sSolitaireDeck[i].p_Card != nullptr)
            {
                head->sSolitaireDeck[i].p_Card->setFaceUp(head->sSolitaireDeck[i].faceUp);
                solitaireDeck.addCard(*head->sSolitaireDeck[i].p_Card);
            }
            else
            {i=26;}
        }  
        drawPile.eraseDeck();
        std::cout << "draw undo\n";
        for (int i=0; i<26; i++)
        {
            if(head->sDrawPile[i].p_Card != nullptr)
            {
                head->sDrawPile[i].p_Card->setFaceUp(head->sDrawPile[i].faceUp);
                drawPile.addCard(*head->sDrawPile[i].p_Card);
            }
            else
            {i=26;}
        }    
            // Don't forget to release the memory allocated for the 'previousNode' if necessary.
            delete previousNode;
    } 
    else 
    {
        std::cout << "No Head\n";
    }
}