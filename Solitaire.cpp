#include "Solitaire.h"
#include <array>
#include <vector>


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

void Solitaire::printGame()
{
    for (int row=0; row<7; row++)                   // there are 7 rows of cards
    {
        for (int i=0; i<row; i++) {cout << "   ";}  // if we are past the first row add blank space in front
        for (int col=0; col<7; col++)               // go through the seven cardCols
        {
            if (row < cardCol[col].getSize(cardCol[col])) //if the row is less than the cardCol size
            {
                Card* c = cardCol[col].getCard(row);
                c->printCard();
                cout << " ";
            }
            if (col+1 > cardCol[col].getSize(cardCol[col]) && row == col) // this pads out for cards that have been sent to an Ace stack
            {
                std::cout << "    ";
            }
        }
        cout << "\n";
    }
}



int Solitaire::cycleDeck()
{
    if (solitaireDeck.cardsLeft() == 0 && getDrawPileSize() > 0){reuseDeck();}
    for (int i=0; i<3; i++)
    {
        if (solitaireDeck.cardsLeft() > 0)
        {
            Card c = solitaireDeck.deal();
            c.flipFaceUp();
            drawPile.addCard(c);
            dpSize ++;
            c.printCard();
            cout << " draw pile size: " << dpSize << "\n";
        }
    }
    
    std::cout << "\033[0m" << "top card: ";
    
    drawPile.printTopCard();
    std::cout << "\033[0m" <<"\n";
        
    int remaining = solitaireDeck.cardsLeft();
    std::cout << "Cycle deck cards remaining = " << remaining << "\n";
    return remaining;
}



Card Solitaire::getTopDrawPileCard()
{
    Card c = drawPile.getDeckCardAt(dpSize-1);
    return c;
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


int Solitaire::getColumnSize(int col)
{
    int size = cardCol[col].getSize(cardCol[col]);
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
void Solitaire::checkCanMove(Card* p_c, int col, int row, bool lastCard)
{
    bool cardRed;
    bool columnRed;
    bool sameCardClicked = false;
    char suits[4] = {'C', 'S', 'H', 'D'};
    int moveSize;
    bool aceMatch = false;
    Card c = *p_c;
   
    if (p_c->getFaceUp() == true)                      // if the card is not flipped up, disregard it
    {
        int id = p_c->getID();                         // from the card's ID, face value and suit

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

        /************ check each of the four aceStacks at the top to see if the card can move here ***********/
        for (int j=0; j<4; j++)                                    // check if a card can play on an ace stack at the top of the table
        {
            // if the array is not empty, a card has not been moved to aces and it is the last card in the cardCol
            std::cout << "aceStack" << j << " size = " << Aces[j].getSize(Aces[j])<< " LC = " << lastCard << " AM = " << aceMatch << "\n";
            if (Aces[j].getSize(Aces[j]) > 0 && lastCard == true && aceMatch == false)  
            {
                int aceID = Aces[j].getLastCardID();             // get the id of the last card in the Aces stack
                int aceStackValue = aceID%13;
                char aceSuit = Aces[j].getLastCardSuit();        // as well as its suit
                int cardID = id%13;
                if (cardID == 0) {cardID = 13;}
                std::cout << "aceStack last card= " << aceStackValue << " incoming card = " << cardID << "\n";
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
            int size =cardCol[i].getSize(cardCol[i]);
            if (size == 0 && cardValue == 13)
            {
                possibleMoves.push_back(i);
            }
        }
    }

    moveSize = possibleMoves.size();
    std::cout << "Possible moves: " ;
    for (int i=0; i<moveSize; i++)
    {
        std::cout << possibleMoves[i] << "/";
    }
    std::cout << "\n";

    /*****************  determine where the card will move to if it is possible for the card to move ****************/
    for (int slot=0; slot<moveSize; slot++)     // go through each possible move
    {
        std::cout << "slot = " << slot << " move size = " << moveSize << " CardCycle = " << cardCycle << "\n";
        int destination = possibleMoves[slot];
        std::cout << "destination = " << destination << "\n";
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
}



/******************  this will move the card to a column in the play area  *******************/
void Solitaire::moveToColumn(int destinationCol, int col, int slot, bool lastCard)
{
    std::cout << "In MoveToColumn\n";
    if (col < 100)                              // this means that it came from the play area and not aces or draw pile
    {
        int colSize = cardCol[col].getSize(cardCol[col]);
        std::cout << "row: " << slot << " col size: " << colSize << " last card: " << lastCard << "\n";
        for (int i= slot; i<colSize; i++)                    // take each card from the selected card to the end
        {
            std:: cout << "ROW: " << i << "\n";
            Card c = cardCol[col].removeCard(slot);                     // remove it from the sorce and..
            cardCol[destinationCol].addCard(c);                      // add it to the destination
        }
        int colLength = cardCol[col].getSize(cardCol[col]);
        std::cout << "after moving cards colLength = " << colLength << "\n";
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
        std::cout << "Draw Pile Card\n";
        playFromDrawPile(destinationCol);
        
    }
        else                    // this is coming from the aces
        {
            std::cout << "aceStack card\n";
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

    int remaining = dpSize;
    std::cout << "****remaining: " << remaining << "\n";
    for (int i=0; i<remaining; i++ )
    {
        Card c = drawPile.getDeckCardAt(i);
        c.printCard();
    }
}


void Solitaire::playFromAces(int col, int suit)
{
    std::cout << "In Play from Aces\n";
    int size = Aces[suit].getSize(Aces[suit]);
    Card c = Aces[suit].removeCard(size-1);
    cardCol[col].addCard(c);  
}


void Solitaire::aceStackMove(int col, int row, int suit, Card* p_c, bool lastCard)
{
    Card inCard = *p_c;

    std::cout << "In AceStackMove Suit = " << suit << " Col = " << col << " Row = " << row << "\n";
    std::cout << "Incomming card is: ";
    inCard.printCard();
    std::cout << "\n";

    Card c;
    if (col<100)
    {      
        c = removeColCard(col, row, true);                     // remove the card from the cardCol    
    }
    else
    {
        c = drawPile.deal();
        dpSize --;
    }
    Aces[suit].addCard(c);                                    // move the card to the Ace stack
    aceFlag = true;
    win = true;
    for (int i=0; i<4; i++)
    {
        int aceSize = Aces[i].getSize(Aces[i]);
        if (aceSize < 13) {win = false;}
    }
    std::cout << "win = " << win << " Out of AceStackMove\n";
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
    for (int slot=row; slot<length; slot++)     // remove each card from the column
    {
        c = cardCol[col].removeCard(slot);
    }
    std::cout << "row = " << row << "\n";
    if (row > 0 && lastCard == true)
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
