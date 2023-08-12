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



int Solitaire::cycleDeck()
{
    if (solitaireDeck.cardsLeft() == 0 && getDrawPileSize() > 0){reuseDeck();}
    int cardsDelt = 0;
    cout << "the drawPile address is: " << &drawPile << endl;;
    for (int i=0; i<3; i++)
    {
        if (solitaireDeck.cardsLeft() > 0)
        {
            Card c = solitaireDeck.deal();          // get and remove the Card object from the solitaireDeck
            c.flipFaceUp();                         // set it's face up bool to true
            int id = c.getID();
            Card* p_c = &c;
            cout << "\nthe card delt's id is " << id << endl;;
            cout << "the delt cards address is " << &p_c << endl;;
            drawPile.addCard(c);                    // add the Card object to the deck drawPile
            dpSize ++;                              // increment dpSize and cardsDelt
            cardsDelt ++;
            c.printCard();
            cout << " draw pile size: " << dpSize << endl;;
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
    return cardsDelt;                               // this let's the calling function know how many cards were delt
}



Card* Solitaire::getTopDrawPileCard()
{
    cout << "GTCDP deck = " << &drawPile << endl;;
    Card* p_c = drawPile.getTopDeckCard();
    drawPile.printTopCard();
    cout << "***** GTCDP Card Address = " << &p_c << "\n\n\n";
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



/*******************************************************************************************
 *          This is the setion that looks over the ID number and suit of a card sent to it         
 *          to check if that card can be played on either a colmn or the aces above         
********************************************************************************************/
bool Solitaire::checkCanMove(Card* p_c, int col, int row, bool lastCard)
{
    bool cardRed;
    bool columnRed;
    bool sameCardClicked = false;
    bool canMove = false;
    char suits[4] = {'C', 'S', 'H', 'D'};
    int moveSize;
    bool aceMatch = false;
    //Card c = *p_c;

    possibleMoves.clear();
    cout << "CheckCanMove's Card address is: " << &p_c << endl;;
    bool fu = p_c->getFaceUp();
    cout << "the card in checkCanMove's facuUp value is: "  << fu << endl;;

    if (p_c->getFaceUp() == true)                      // if the card is not flipped up, disregard it
    {
        int id = p_c->getID();                         // from the card's ID, face value and suit
        cout << "CheckPosMoves id = " << id << endl;;

        /********  check if the same card was clicked again  *******/
        if (id == lastCardClicked)                  // if the same card is clicked again
        {
            sameCardClicked = true;                 // if the same card is clicked again      
            cardCycle ++;                           // increment the counter
            moveSize = possibleMoves.size();
            if (cardCycle >= moveSize)  // if the counter has reached the possible spots for the card
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
        for (int j=0; j<4; j++)                                    // check if a card can play on an ace stack at the top of the table
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
                    moveToColumn(destination, col, row, lastCard); 
                    moved = true;
                }
            }             
        }
    }
    return canMove;
}



/******************  this will move the card to a column in the play area  *******************/
void Solitaire::moveToColumn(int destinationCol, int col, int slot, bool lastCard)
{
    cout << "In MoveToColumn\n";
    if (col < 100)                              // this means that it came from the play area and not aces or draw pile
    {
        int colSize = cardCol[col].getSize();
        cout << "row: " << slot << " col size: " << colSize << " last card: " << lastCard << endl;;
        for (int i= slot; i<colSize; i++)                    // take each card from the selected card to the end
        {
            std:: cout << "ROW: " << i << endl;;
            Card c = cardCol[col].removeCard(slot);                     // remove it from the sorce and..
            cardCol[destinationCol].addCard(c);                      // add it to the destination
        }
        int colLength = cardCol[col].getSize();
        cout << "after moving cards colLength = " << colLength << endl;;
        if (colLength > 0)                                          // if there are still cards in the col, flip one
        {
            Card* t = cardCol[col].getCard(colLength-1);
            if (lastCard == true) {t->flipFaceUp();}
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
}


void Solitaire::playFromDrawPile(int col)
 { 
    bool canMove;
    
    if(dpSize > 0) 
    {       
        Card c = drawPile.deal();
        cardCol[col].addCard(c);
        dpSize --;
    }
}


void Solitaire::playFromAces(int col, int suit)
{
    cout << "In Play from Aces\n";
    int size = Aces[suit].getSize();
    Card c = Aces[suit].removeCard(size-1);
    cardCol[col].addCard(c);  
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
        dpSize --;
    }
    Aces[suit].addCard(c);                                    // move the card to the Ace stack
    aceFlag = true;

    win = true;                                               // this checks for the win condition
    for (int i=0; i<4; i++)                                   
    {
        int aceSize = Aces[i].getSize();
        if (aceSize < 13) {win = false;}
    }
    cout << "win = " << win << " Out of AceStackMove\n";
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


Card Solitaire::removeColCard(int col, int row, bool lastCard)
{
    Card c;
    int length = getColumnSize(col);
        cout << "** RemoveColCard row = " << row << " length = " << length << endl;;
    for (int slot=row; slot<length; slot++)     // remove each card from the column
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
        c.flipCard();
        solitaireDeck.addCard(c);
    }
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
 }
