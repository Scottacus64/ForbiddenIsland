#include "SolitaireUI.h"
#include <string>
#include <iostream>

#include <QCoreApplication>
#include <QDir>

SolitaireUI::SolitaireUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SolitaireUI)
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString assetPath = QDir::cleanPath(appDir + QDir::separator() + "pngs") + QDir::separator();

    ui->setupUi(this);

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
            m_pC[j+(k*19)]->setGeometry(QRect(130+(k*110), 160+(j*30), 100, 140));
            m_pC[j+(k*19)]->setIcon(QPixmap(QString::fromUtf8("../../VSC/CPP/Solitaire/CardPNGs/13S.png")));
            m_pC[j+(k*19)]->setText(QString());
            m_pC[j+(k*19)]->setStyleSheet(QString::fromUtf8("border: none"));
            m_pC[j+(k*19)]->setEnabled(false);
            QSize iconSize(100, 140);
            m_pC[j+(k*19)]->setIconSize(iconSize);
            connect(m_pC[j+(k*19)], &QPushButton::clicked, this, &SolitaireUI::cardClicked);
        }
    }

        // set up the two deck piles at the bottom
    for (int i = 0; i < 2; i++) 
    {
        std::string name = "D" + std::to_string(i);
        m_pD[i] = new QPushButton(QString::fromStdString(name), this);
        m_pD[i]->setObjectName(QString::fromStdString(name));
        m_pD[i]->setGeometry(QRect(390 + (i * 110), 750, 100, 140));
        m_pD[i]->setText(QString());
        
        QSize iconSize(100, 140);
        m_pD[i]->setIconSize(iconSize);
        connect(m_pD[i], &QPushButton::clicked, this, &SolitaireUI::cardClicked);
    }
    m_pD[0]->setIcon(QPixmap(cardImage[0]));
    m_pD[0]->setEnabled(true); 
    m_pD[1]->setIcon(QPixmap()); 
    m_pD[1]->setEnabled(false);

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
    m_pD[1]->setEnabled(false);
    m_pD[1]->setIcon(QPixmap());
    m_pD[1]->setText(QString());
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
        for (int card=0; card<column.getSize(column); card++)
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
            m_pSolitaire->checkCanMove(p_c, col, row, lastCard);
        }
        /*********** if the aceStack is clicked **************/
        else if(firstChar == 'A')
        {
            std::cout << "Clicked an A column\n";
            int suit = secondChar.digitValue();
            Hand* p_aceStack = m_pSolitaire->getAceStack(suit);
            Hand aceStack = *p_aceStack;
            int size = aceStack.getSize(aceStack);
            
            Card* p_c = aceStack.getCard(size-1);
            m_pSolitaire->checkCanMove(p_c, 100, suit, true);
        }
        /*********** if the draw pile or deck stack is clicked *************/
        else if(firstChar == 'D')
        {
            
            std::string deck = cardName.toStdString();
            std::cout << "Clicked on Deck" << deck << "\n";
            if (deck == "D0") 
            {
                int active = m_pSolitaire->cycleDeck();
                if (active > 0) {updateDecks(0, 1); updateDecks(1,1);}
                else {updateDecks(0,0);updateDecks(1,1);}
            }
            else 
            {
                int size = m_pSolitaire->getDrawPileSize();
                std::cout << "UI drawPile size = " << size << "\n";

                if (size > 0) 
                {
                    Card c = m_pSolitaire->getTopDrawPileCard();
                    Card* p_c = &c;
                    m_pSolitaire->checkCanMove(p_c, 100, 100, true);
                    std::cout << "checking from draw pile\n" ;
                    int endSize = m_pSolitaire->getDrawPileSize();
                    endSize > 0 ? updateDecks(1, 1) : updateDecks(1,0);
                }
                else 
                {
                    updateDecks(1,0);
                    std::cout << "Draw Pile empty\n";
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
    }
}

void SolitaireUI::updateDecks(int deck, int cards)
{
    if (deck == 0)
    {
        if (cards > 0)
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
    if (cards >0)
        {
            m_pD[1]->setEnabled(true);
            Card card = m_pSolitaire->getTopDrawPileCard();
            int id = card.getID();
            m_pD[1]->setText(QString());
            m_pD[1]->setIcon(QPixmap(cardImage[id]));
        }
        else
        {
            m_pD[1]->setEnabled(false);
            m_pD[1]->setText(QString());
            m_pD[1]->setIcon(QPixmap()); 
        }        
    }
    /*bool win = m_pSolitaire->getWin();
    std::cout << "Win = " << win << "\n";
    if (win == true)
    {
        m_pD[0]->setEnabled(false);
        m_pD[0]->setText("WIN!!");
        m_pD[1]->setEnabled(false);
        m_pD[1]->setIcon(QPixmap()); 
        m_pD[1]->setText("WIN!!");
    }*/
}

void SolitaireUI::checkForWin()
{
    bool win = m_pSolitaire->getWin();
    std::cout << "Win = " << win << "\n";
    if (win == true)
    {
        m_pD[0]->setEnabled(false);
        m_pD[0]->setText("WIN!!");
        m_pD[1]->setEnabled(false);
        m_pD[1]->setIcon(QPixmap()); 
        m_pD[1]->setText("WIN!!");
    } 
}
