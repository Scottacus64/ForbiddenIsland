#include "SolitaireUI.h"
#include <string>
#include <iostream>
#include <QPainter>
#include <QTimer>

#include <QCoreApplication>
#include <QDir>

SolitaireUI::SolitaireUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SolitaireUI)
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString assetPath = QDir::cleanPath(appDir + QDir::separator() + "pngs") + QDir::separator();

    ui->setupUi(this);

    green = QPixmap(assetPath + "green.png");
    // set up all of the card image QPixmaps
    cardImage[0] = QPixmap(assetPath + "0B.png");
    char suits[4] = {'C', 'S', 'H', 'D'};
    int slot = 0;
    for (int s=0; s<4; s++)
    {
        for (int i=1; i<14; i++)
        {
            slot++;
            cardImage[slot] = QPixmap(assetPath + QString::number(i) + suits[s]+ ".png");
        }
    }

    // set up the four Ace piles at the top
    for (int i = 0; i < 4; i++) 
    {
        std::string name = "A" + std::to_string(i);
        m_pA[i] = new QPushButton(QString::fromStdString(name), this);
        m_pA[i]->setObjectName(QString::fromStdString(name));
        m_pA[i]->setGeometry(QRect(290 + (i * 110), 0, 100, 140));
        m_pA[i]->setIcon(QPixmap()); 
        m_pA[i]->setText(QString());
        m_pA[i]->setEnabled(false);
        QSize iconSize(100, 140);
        m_pA[i]->setIconSize(iconSize);
        connect(m_pA[i], &QPushButton::clicked, this, &SolitaireUI::cardClicked);
    }

    // set up all of the columns of cards in the playfield area
    for (int k=0; k<7; k++) 
    {
        for (int j=0; j<19; j++) 
        {
            std::string name = "C" + std::to_string(j+(k*19));
            m_pC[j+(k*19)] = new QPushButton (QString::fromStdString(name), this);
            m_pC[j+(k*19)]->setObjectName(QString::fromStdString(name));
            m_pC[j+(k*19)]->setGeometry(QRect(130+(k*110), 160+(j*25), 100, 140));
            m_pC[j+(k*19)]->setIcon(QPixmap(QString::fromUtf8("../../VSC/CPP/Solitaire/CardPNGs/13S.png")));
            m_pC[j+(k*19)]->setText(QString());
            m_pC[j+(k*19)]->setStyleSheet(QString::fromUtf8("border: none"));
            m_pC[j+(k*19)]->setEnabled(false);
            QSize iconSize(100, 140);
            m_pC[j+(k*19)]->setIconSize(iconSize);
            connect(m_pC[j+(k*19)], &QPushButton::clicked, this, &SolitaireUI::cardClicked);
        }
    }

        // set up the deck and draw piles at the bottom
    int j=0;
    int k=10;    
    for (int i = 0; i < 4; i++) 
    {
        if (i>0){j=110;}
        std::string name = "D" + std::to_string(i);
        m_pD[i] = new QPushButton(QString::fromStdString(name), this);
        m_pD[i]->setObjectName(QString::fromStdString(name));
        m_pD[i]->setGeometry(QRect(290+j+k, 750, 100, 140));
        m_pD[i]->setText(QString());
        k+=20;
        QSize iconSize(100, 140);
        m_pD[i]->setIconSize(iconSize);
        connect(m_pD[i], &QPushButton::clicked, this, &SolitaireUI::cardClicked);
    }
    m_pD[0]->setIcon(QPixmap(cardImage[0]));
    m_pD[0]->setEnabled(true); 
    for(int i=1; i<4; i++)
    {
        m_pD[i]->setIcon(QPixmap()); 
        m_pD[i]->setEnabled(false);
        m_pD[i]->hide();
    }

    std::string name = "NewGame";
    m_newGame = new QPushButton(QString::fromStdString(name), this);
    m_newGame->setObjectName(QString::fromStdString(name));
    m_newGame->setGeometry(QRect(700, 750, 140, 100));
    m_newGame->setText(QString("New Game?"));
    connect(m_newGame, &QPushButton::clicked, this, &SolitaireUI::cardClicked);

    m_pSolitaire = new Solitaire();

    dealCards();
}


SolitaireUI::~SolitaireUI()
{
    delete ui;
}


void SolitaireUI::dealCards()
{
    m_pSolitaire->dealGame();
    refreshScreen();
    m_pD[0]->setEnabled(true);
    m_pD[0]->setIcon(QPixmap(cardImage[0]));
    m_pD[0]->setText(QString());
    for (int i=1; i<4; i++)
    {
        m_pD[i]->setEnabled(false);
        m_pD[i]->setIcon(QPixmap());
        m_pD[i]->setText(QString());
        m_pD[i]->hide();
    }
}


void SolitaireUI::refreshScreen()
{
    for (int i=0; i<133; i++) 
    {
        m_pC[i]->setEnabled(false); 
        m_pC[i]->setIcon(QPixmap());
    }

    for (int col=0; col<7; col++)
    {
        Hand column = m_pSolitaire->getColumn(col);
        for (int card=0; card<column.getSize(); card++)
        {
            Card* p_c = column.getCard(card);
            int id = p_c->getID();              // id is the same as the cardImage value
            int pbCard = (col * 19) + card;     // there are 19 cards in a column
            m_pC[pbCard]->setEnabled(true);     // turn on the QPushButton enabled so it can be clicked
            m_pC[pbCard]->raise();              // raise the card above the hidden cards to prevent them from...
            if (p_c->getFaceUp() == true)       // intercepting mouse clicks
            {
                m_pC[pbCard]->setIcon(QPixmap(cardImage[id]));
            }
            else
            {
                m_pC[pbCard]->setIcon(QPixmap(cardImage[0]));
            }
        }
    }

    for (int i=0; i<4; i++)
    {
        int id = m_pSolitaire->getTopAce(i);
        if (id > 0)
        {
            m_pA[i]->setIcon(QPixmap(cardImage[id]));
            m_pA[i]->setEnabled(true);
        }
        else
        {
            m_pA[i]->setIcon(QPixmap());
            m_pA[i]->setEnabled(false);
        }
    }
}


void SolitaireUI::cardClicked()
{
    QPushButton* clickedCard = qobject_cast<QPushButton*>(sender());
    if (clickedCard) 
    {
        int dpSize = m_pSolitaire->getDrawPileSize();           // this allows expansion of the last draw piles to register
        if (dpSize > 2 && drawPileFlag == true ){cardsDelt = 3;}

        std::cout << "** Top of card clicked cardsDelt = " << cardsDelt << endl;;
        bool lastCard = false;
        QString cardName = clickedCard->objectName();
        QChar firstChar = cardName.at(0);
        QChar secondChar = cardName.at(1);
        if (firstChar == 'C')
        {
            cardName.remove(0, 1);
            std::string card = cardName.toStdString();
            int slot = std::stoi(card);
            int col = slot/19;
            int row = slot%19;
            Card* p_c = m_pSolitaire->getColCardAt(col, row);
            Card c = *p_c;
            int cardID = c.getID();
            int colLastCardID = m_pSolitaire->getColumn(col).getLastCardID();
            if (cardID == colLastCardID) {lastCard = true;}
            if (row > 0)
            {
                Card* p_test = m_pSolitaire->getColCardAt(col, row-1);
                bool flipped = p_test->getFaceUp();
                if (flipped == false) {lastCard = true;}
            }
            drawPileFlag = false;
            m_pSolitaire->checkCanMove(p_c, col, row, lastCard);
        }
        /*********** if the aceStack is clicked **************/
        else if(firstChar == 'A')
        {
            std::cout << "Clicked an A column\n";
            int suit = secondChar.digitValue();
            Hand* p_aceStack = m_pSolitaire->getAceStack(suit);
            Hand aceStack = *p_aceStack;
            int size = aceStack.getSize();
            
            Card* p_c = aceStack.getCard(size-1);
            drawPileFlag = false;
            m_pSolitaire->checkCanMove(p_c, 100, suit, true);
        }
        /*********** if the draw pile or deck stack is clicked *************/
        else if(firstChar == 'D')
        {
            for (int i=0; i<4; i++) {m_pD[i]->show();}
            std::string deck = cardName.toStdString();
            std::cout << "Clicked on " << deck << endl;;
            if (deck == "D0") 
            {
                cardsDelt = m_pSolitaire->cycleDeck();                              // deals cards and returns how many
                if (cardsDelt > 0) {updateDecks(0, 1); updateDecks(1,cardsDelt);}   // sets up the cards in draw piles
                Deck* p_deck = m_pSolitaire->getDeck();
                int dSize = p_deck->cardsLeft();
                drawPileFlag = false;
                if (dSize == 0) {updateDecks(0,0);}                                 // if the deck is empty update that data
            }
            else 
            {
                int dPiles = secondChar.digitValue();               // get the number of the pile clicked
                int dpSize = m_pSolitaire->getDrawPileSize();       // get the size of the draw pile
                
                if (dpSize < 3) {cardsDelt = dpSize;}               // if there are less than 3 cards in dp set cardsDelt to that number
                std::cout << "**** CardClicked cardsDelt = " << cardsDelt << " dpSize = " << dpSize << endl;;
                if (dPiles == cardsDelt)                            // if the pile clicked is the active pile
                {
                    if (dpSize > 0) 
                    {
                        Card* p_c = m_pSolitaire->getTopDrawPileCard();
                        bool canMove = m_pSolitaire->checkCanMove(p_c, 100, 100, true);
                        if (canMove==true) {updateDecks(1,m_pSolitaire->getDrawPileSize());}    // needs a fresh dpSize
                    }
                    else 
                    {
                        updateDecks(1,0);
                        std::cout << "Draw Pile empty\n";
                    }
                }
            }
        }
        else            // must be in newGame
        {
            std::cout << "New Game\n";
            m_pSolitaire->playGame();
            dealCards();
        }
        checkForWin();
        refreshScreen();
        bool aFinish = m_pSolitaire->checkAutoFinish();
        if (aFinish == true){autoFinish();}
        m_pSolitaire->printField();
    }
}


void SolitaireUI::autoFinish()
{
    std::cout << "In autoFinish \n";
    bool gameDone = false;
    while (gameDone == false)
    {
        gameDone = m_pSolitaire->nextCard();
        refreshScreen();
        delayTimer(200);
    }
}


void SolitaireUI::updateDecks(int deck, int cDelt)
{
    if (deck == 0)
    {
        if (cDelt > 0)
        {
            m_pD[0]->setEnabled(true);
            m_pD[0]->setIcon(QPixmap(cardImage[0]));
            m_pD[0]->setText(QString());
        }
        else
        {
            m_pD[0]->setIcon(QPixmap());
            m_pD[0]->setText("Again?");  
        }
    }
    else
    {
    if (cDelt > 0)
        {
            for (int i=1; i<4; i++){disableDrawPile(i);}            // disable each draw pile to clear everything
            if (cDelt > 2){cDelt = 3;}
            int dpSize = m_pSolitaire->getDrawPileSize();           // get the size of the drawPile
            std::cout << "** UpdateDecks cards delt = " << cDelt << " dpSize = " << dpSize <<endl;;
            for (int i=1; i<cDelt+1; i++)                           // go from D1 to D3
            {
                int slot = dpSize-cDelt-1+i;                        // start with the -2 to 0 slot from the back
                std::cout << " slot = "<< slot<< endl;;
                Card* p_card = m_pSolitaire->getDrawPileAt(slot);   // get the Card pointer for this slot
                int id = p_card->getID();                           // get the card's id
                enableDrawPile(i, id);                              // enable that draw pile deck stack
            }
            drawPileFlag = true;
            std::cout << "**** UpdateDecks dpSize = " << dpSize << " cardsDelt = " << cardsDelt << endl;;
        }
        else
        {
            m_pD[1]->setEnabled(false);
            m_pD[1]->setText(QString());
            m_pD[1]->setIcon(QPixmap()); 
        }        
    }
}


void SolitaireUI::checkForWin()
{
    bool win = m_pSolitaire->getWin();
    std::cout << "Win = " << win << endl;;
    if (win == true)
    {
        m_pD[0]->setEnabled(false);
        m_pD[0]->setText("WIN!!");
        m_pD[1]->setEnabled(false);
        m_pD[1]->setIcon(QPixmap()); 
        m_pD[1]->setText("WIN!!");
    } 
}


void SolitaireUI::enableDrawPile(int pile, int id)
{
    m_pD[pile]->setEnabled(true);
    m_pD[pile]->setIcon(QPixmap(cardImage[id]));
    m_pD[pile]->raise();
}


void SolitaireUI::disableDrawPile(int pile)
{
    m_pD[pile]->setEnabled(false);
    m_pD[pile]->setIcon(QPixmap());
    m_pD[pile]->setStyleSheet(QString::fromUtf8("border: none"));
    if (pile>1)
    {
        m_pD[pile-1]->setEnabled(true);
        m_pD[pile]->lower();              // lower the card 
    }
}

void SolitaireUI::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Paint the green background image
    painter.drawPixmap(0, 0, width(), height(), green);

    // ... your existing painting code
}


void SolitaireUI::delayTimer(int delay)
{
    QTimer timer;
    timer.setInterval(delay); 
    timer.start();
    // Use a loop to wait until the QTimer times out (25ms)
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
}