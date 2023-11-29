#include "SolitaireUI.h"
#include <string>
#include <iostream>
#include <QPainter>
#include <QTimer>
#include <QFont>
#include <QPalette>
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
    // set up the buttons
    m_newGame = new QPushButton("newGame", this);
    m_newGame->setObjectName(QString::fromStdString("newGame"));
    m_newGame->setGeometry(QRect(600, 740, 140, 50));
    m_newGame->setText(QString("New Game?"));
    connect(m_newGame, &QPushButton::clicked, this, &SolitaireUI::cardClicked);

    m_undo = new QPushButton("undo", this);
    m_undo->setObjectName("undo");
    m_undo->setGeometry(QRect(600, 800, 140, 50));
    m_undo->setText(QString("Undo"));
    connect(m_undo, &QPushButton::clicked, this, &SolitaireUI::undoPressed);

    m_easy = new QPushButton("easy", this);
    m_easy->setObjectName(QString::fromStdString("easy"));
    m_easy->setGeometry(QRect(800, 740, 140, 50));
    m_easy->setText(QString("Easy"));
    connect(m_easy, &QPushButton::clicked, this, &SolitaireUI::easyClicked);

    m_medium = new QPushButton("medium", this);
    m_medium->setObjectName(QString::fromStdString("medium"));
    m_medium->setGeometry(QRect(800, 800, 140, 50));
    m_medium->setText(QString("Medium"));
    connect(m_medium, &QPushButton::clicked, this, &SolitaireUI::mediumClicked);

    m_hard = new QPushButton("hard", this);
    m_hard->setObjectName(QString::fromStdString("hard"));
    m_hard->setGeometry(QRect(800, 860, 140, 50));
    m_hard->setText(QString("Hard"));
    connect(m_hard, &QPushButton::clicked, this, &SolitaireUI::hardClicked);

    // set up the labels
    QFont font;
    font.setPointSize(28);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);

    m_moves = new QLabel("moves", this);
    m_moves->setGeometry(QRect(600,900,200,50));
    m_moves->setFont(font);
    m_moves->setPalette(palette);
    m_moves->setText(QString("Moves = "));

    m_timer = new QLabel("Time: ", this);           
    m_timer->setGeometry(QRect(600, 930, 300, 50));
    m_timer->setFont(font);
    m_timer->setPalette(palette);
    //elapsedTimer.start();

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        elapsedMilliseconds = elapsedTimer.elapsed();
        qint64 elapsedSeconds = elapsedMilliseconds / 1000;
        QString elapsedTimeStr = QString("Time: %1 seconds").arg(elapsedSeconds);
        m_timer->setText(elapsedTimeStr);
    });

    //timer.start(1000);

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
    refreshUpperSection();
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
    gameStarted = false;
    timer.stop();
    elapsedMilliseconds = 0;
    m_timer->setText("Time:");
}


void SolitaireUI::refreshUpperSection()
{
    for (int i=0; i<133; i++) 
    {
        m_pC[i]->setEnabled(false); 
        m_pC[i]->setIcon(QPixmap());
    }

    for (int col=0; col<7; col++)
    {
        Hand* column = m_pSolitaire->getColumn(col);
        for (int card=0; card<column->getSize(); card++)
        {
            Card* pCard = column->getCard(card);
            int id = pCard->getID();              // id is the same as the cardImage value
            int pbCard = (col * 19) + card;     // there are 19 cards in a column
            m_pC[pbCard]->setEnabled(true);     // turn on the QPushButton enabled so it can be clicked
            m_pC[pbCard]->raise();              // raise the card above the hidden cards to prevent them from...
            if (pCard->getFaceUp() == true)       // intercepting mouse clicks
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
    int moves = m_pSolitaire->getMoves();
    m_moves->setText(QString("Moves = ") + QString::number(moves));
}


void SolitaireUI::cardClicked()
{
    QPushButton* clickedCard = qobject_cast<QPushButton*>(sender());
    if (clickedCard) 
    {
        if (gameStarted == false){elapsedTimer.start(); timer.start(1000);}
        gameStarted = true;
        std::cout << "** Top of card clicked cardsDelt = " << cardsDelt << endl;;
        bool lastCard = false;
        bool lastUnflippedCard = false;
        QString cardName = clickedCard->objectName();
        QChar firstChar = cardName.at(0);
        QChar secondChar = cardName.at(1);
        if (firstChar == 'C')
        {
            cardName.remove(0, 1);                                              //strip the leading C
            std::string card = cardName.toStdString();
            int slot = std::stoi(card);
            int col = slot/19;
            int row = slot%19;
            Card* pCard = m_pSolitaire->getColCardAt(col, row);                 //find the solitaire card at this location
            int cardID = pCard->getID();                                        //get the card's ID
            int colLastCardID = m_pSolitaire->getColumn(col)->getLastCardID();  //get the ID of the last card in the column
            if (cardID == colLastCardID) {lastCard = true;}                     //if they are equal then we have found the last card
            if (row > 0)
            {
                Card* pTest = m_pSolitaire->getColCardAt(col, row-1);          //this is used if a stack is moved and the last card
                bool flipped = pTest->getFaceUp();                             //in the column is not flipped
                if (flipped == false) {lastUnflippedCard = true;}
            }
            m_pSolitaire->checkCanMove(pCard, col, row, lastCard, lastUnflippedCard);
        }
        /*********** if the aceStack is clicked **************/
        else if(firstChar == 'A')
        {
            std::cout << "Clicked an A column\n";
            int suit = secondChar.digitValue();
            Hand* pAceStack = m_pSolitaire->getAceStack(suit);
            Hand aceStack = *pAceStack;
            int size = aceStack.getSize();
            
            Card* pCard = aceStack.getCard(size-1);
            m_pSolitaire->checkCanMove(pCard, 100, suit, true, false);
        }
        /*********** if the draw pile or deck stack is clicked *************/
        else if(firstChar == 'D')
        {
            for (int i=0; i<4; i++) {m_pD[i]->show();}
            std::string deck = cardName.toStdString();
            std::cout << "Clicked on " << deck << endl;;
            if (deck == "D0") 
            {
                cardsDelt = m_pSolitaire->cycleDeck();                  // deals cards and returns how many
                int drawPileSize = m_pSolitaire->getDrawPileSize();
                if (cardsDelt > 0) {updateDecks(1,drawPileSize);}       // sets up the cards in draw piles
                int dSize = m_pSolitaire->getDeckSize();
                if (dSize == 0) 
                    {updateDecks(0,0);}                                 // if the deck is empty update that data
                else
                    {updateDecks(0,dSize);}
            }
            else 
            {
                int drawPileSize = m_pSolitaire->getDrawPileSize();
                int activePile = drawPileSize;
                if (drawPileSize >2){activePile = 3;}
                int dPiles = secondChar.digitValue();               // get the number of the pile clicked
                if (dPiles == activePile)                            // if the pile clicked is the active pile
                {
                    if (drawPileSize > 0) 
                    {
                        Card* pCard = m_pSolitaire->getTopDrawPileCard();
                        bool canMove = m_pSolitaire->checkCanMove(pCard, 100, 100, true, false);
                        std::cout << "CanMove = " << canMove << "\n";
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

        refreshUpperSection();
        bool aFinish = m_pSolitaire->checkForWin();
        if (aFinish == true){autoFinish();}
        m_pSolitaire->printField();
        bool canPlay = m_pSolitaire->checkCanPlay();
        if (canPlay == true)
            {
                m_newGame->setText(QString("True"));
            }
        else{
                m_newGame->setText(QString("False"));
            }
    }
}


void SolitaireUI::autoFinish()
{
    std::cout << "In autoFinish \n";
    bool gameDone = false;
    bool win = false;
    while (gameDone == false)                       // cycle through all remaining cards and play them to the Aces
    {
        gameDone = m_pSolitaire->nextCard();
        refreshUpperSection();
        delayTimer(200);
    }
    win = m_pSolitaire->checkForWin();
    std::cout << "******* In UI AutoFinish win = " << win << "\n";
    if (win == true) {postWin();}
}


void SolitaireUI::postWin()
{
    timer.stop();
    m_pD[0]->setEnabled(false);
    m_pD[0]->setText("WIN!!");
    m_pD[1]->setEnabled(false);
    m_pD[1]->setIcon(QPixmap()); 
    m_pD[1]->setText("WIN!!");
}


void SolitaireUI::enableDrawPile(int pile, int id)              // this method makes a draw pile show up graphically
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

void SolitaireUI::undoPressed()
{
    std::cout << "Undo button pressed\n";
    m_pSolitaire->loadGameState();
    refreshUpperSection(); 
    refreshDecks();
}


void SolitaireUI::updateDecks(int deck, int dCards)
{
    std::cout << "********** Update Decks deck: " << deck << " dCards " << dCards << endl;
    if (deck == 0)
    {
        if (dCards > 0)
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
    if (dCards > 0)
        {
            
            for (int i=1; i<4; i++){disableDrawPile(i);}       // disable each draw pile to clear everything
            int dpSize = m_pSolitaire->getDrawPileSize();      // get the size of the drawPile
            int dPiles = dpSize;
            if (dPiles> 2) {dPiles = 3;}
            std::cout << "********** UpdateDecks dpSize = " << dpSize << " dPiles = " << dPiles <<endl;;
            for (int i=0; i<dPiles; i++)                        // go from D1 to D3
            {
                Card* pCard = m_pSolitaire->getDrawPileAt(dpSize-dPiles+i);   // get the Card pointer for this slot
                int id = pCard->getID();                        // get the card's id
                enableDrawPile(i+1, id);                        // show that draw pile deck stack and card
                std::cout << "********** " << pCard->getPrintValue() << pCard->getSuit() << "\n";
            }

        }
        else
        {
            m_pD[1]->setEnabled(false);
            m_pD[1]->setText(QString());
            m_pD[1]->setIcon(QPixmap()); 
        }        
    }
}


void SolitaireUI::refreshDecks()
{
    int deckSize = m_pSolitaire->getDeckSize();
    if (deckSize > 0)                                       // This is the main solitaire deck
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
    int drawPileSize = m_pSolitaire->getDrawPileSize();           // this is for the draw pile and the flipped cards
    for (int i=1; i<4; i++){disableDrawPile(i);}                  // disable each draw pile to clear everything
    int drawPiles = drawPileSize;
    if (drawPiles > 3) {drawPiles = 3;}
    if (drawPileSize > 0)                                         // if there are cards in the draw pile
    { 
        for (int i=0; i<drawPiles; i++)                       // cycle through any flipped cards as they are active
        {
            Card* pCard = m_pSolitaire->getDrawPileAt(drawPileSize-drawPiles+i); // the piles from left to right are populated from the top of the deck
            enableDrawPile(i+1,pCard->getID());
        }
    }
}

void SolitaireUI::easyClicked()
{
    m_pSolitaire->makeWinnableDeck();
    refreshUpperSection();
    refreshDecks();
}


void SolitaireUI::mediumClicked()
{
    m_pSolitaire->finishDeck();
    refreshUpperSection();
    refreshDecks();
}


void SolitaireUI::hardClicked()
{
}