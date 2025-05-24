#include "ForbiddenIslandUI.h"
#include <string>
#include <iostream>
#include <QPainter>
#include <QTimer>
#include <QFont>
#include <QPalette>
#include <QCoreApplication>
#include <QDir>
#include <QScreen>
#include <fstream>
#include <unistd.h>
#include <QStandardPaths>
#include <QFont>
#include <QFontDatabase>
#include <algorithm>



ForbiddenIslandUI::ForbiddenIslandUI(QWidget *parent)
    : QWidget(parent)

{
    QString appDir = QCoreApplication::applicationDirPath();
    QString assetPath = QDir::cleanPath(appDir + QDir::separator() + "CardPngs") + QDir::separator();

    //ui->setupUi(this);
    this->setObjectName("ForbiddenIslandUI");
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int screenWidth = screenGeometry.width();
    this->resize(1087, 1000);
    this->move(((screenWidth/2) -(1087/2)), 0);

    // Create QLabel and QPushButton instances
    QLabel *c0 = new QLabel(this);
    QLabel *c1 = new QLabel(this);
    QLabel *a0 = new QLabel(this);

    // set up the labels
    QFont font("Franklin Gothic Medium");
    font.setPointSize(42);
    QPalette palette;
    palette.setColor(QPalette::WindowText, QColor(255, 215, 0));

     // Load LNBold font
    int boldFontId = QFontDatabase::addApplicationFont(assetPath + "LNBold.ttf");
    QString boldFontFamily;
    boldFontFamily = QFontDatabase::applicationFontFamilies(boldFontId).at(0);
    QFont boldFont(boldFontFamily);
    boldFont.setPointSize(42);

    QFont boldLargeFont(boldFontFamily);
    boldLargeFont.setPointSize(84);

    // Load LNLight font
    int lightFontId = QFontDatabase::addApplicationFont(assetPath + "LNLight.ttf");
    QString lightFontFamily;
    lightFontFamily = QFontDatabase::applicationFontFamilies(lightFontId).at(0);
    QFont lightFont(lightFontFamily); 
    lightFont.setPointSize(42);

    QFont lightLargeFont(lightFontFamily); 
    lightLargeFont.setPointSize(84);
    QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/water.jpg";
    water = QPixmap(path);
    // set up all of the card image QPixmaps
    const vector <string> cardTreasure = {"WRBC", "EWR", "FWR", "GWR", "HLWR", "LWR",  "SBWR", "WR"};
    const vector <string> cardFlood = {"FIC", "BBC", "BGC", "CAC", "CEC", "CFC", "CGC", "CPC", "CSC", "DDC", "FLC", "GGC", "HGC", "IGC", "LLC", "MMC", "OC", "PRC", "SGC", "THC", "TMC", "TPC", "TSC", "WC","WGC"};
    const vector <string> cardIsland = {"BBB", "BBF", "BGB", "BGF", "CAB", "CAF", "CEB", "CEF", "CFB", "CFF", "CGB", "CGF", "CPB", "CPF", "CSB", "CSF", "DDB", "DDF", "FLB", "FLF", "GGB", "GGF", "HGB", "HGF", "IGB", "IGF", "LLB", "LLF", "MMB", "MMF", "OB", "OF", "PRB", "PRF", "SGB", "SGF", "THB", "THF", "TMB", "TMF", "TPB", "TPF", "TSB", "TSF", "WB", "WF", "WGB", "WGF"};
    const vector <string> cardPlayer = {"DC", "EC", "ExC", "MC", "NC", "PC",};
    const vector <int> invalidSquares = {0,1,4,5,6,11,24,29,30,31,34,35};
    for(int i=0; i<cardTreasure.size(); i++)
    {
        cardImageTreasure[i] = QPixmap(assetPath + QString::fromStdString(cardTreasure[i]) + ".png");
    }
    for(int i=0; i<cardFlood.size(); i++)
    {
        cardImageFlood[i] = QPixmap(assetPath + QString::fromStdString(cardFlood[i]) + ".png");
    }
    for(int i=0; i<cardIsland.size(); i++)
    {
        cardImageIsland[i] = QPixmap(assetPath + QString::fromStdString(cardIsland[i]) + ".png");
    }
    for(int i=0; i<cardPlayer.size(); i++)
    {
        cardImagePlayer[i] = QPixmap(assetPath + QString::fromStdString(cardPlayer[i]) + ".png");
    }

    int y = 0;
    int x = 0;
    int counter = 0;
    for(int i=0; i<36; i++)
    {
        if(i%6 == 0 && i>0)
        {
            y+=1;
            x=0;
        }
        if(std::find(invalidSquares.begin(), invalidSquares.end(), i) != invalidSquares.end()) 
        {
            x+=1;
            continue;
        }
        string name = "iC" + to_string(i);
        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/BBF.png";
        m_iC[counter] = new QPushButton(QString::fromStdString(name), this);
        m_iC[counter]->setObjectName(QString::fromStdString(name));
        m_iC[counter]->setGeometry(QRect(220 + (x*105), 150 + (y*105), 100, 100));
        m_iC[counter]->setIcon(QPixmap(path));//QString::fromUtf8("../../VSC/CPP/ForbiddenIsland/CardPNGs/BBF.png"))); 
        m_iC[counter]->setText(QString());
        m_iC[counter]->setEnabled(true);
        m_iC[counter]->setVisible(true);
        QSize iconSize(100, 100);
        m_iC[counter]->setIconSize(iconSize);
        counter+=1;
        
        x+=1;
        
        //connect(m_iC[i], &QPushButton::clicked, this, &ForbiddenIslandUI::cardClicked);
    }

    // set up the four Ace piles at the top
    /*for (int i = 0; i < 4; i++) 
    {
        std::string name = "A" + std::to_string(i);
        m_pA[i] = new QPushButton(QString::fromStdString(name), this);
        m_pA[i]->setObjectName(QString::fromStdString(name));
        m_pA[i]->setGeometry(QRect(290 + (i * 110), 50, 100, 140));
        m_pA[i]->setIcon(QPixmap()); 
        m_pA[i]->setText(QString());
        m_pA[i]->setEnabled(false);
        m_pA[i]->setVisible(false);
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
            m_pC[j+(k*19)]->setGeometry(QRect(130+(k*110), 210+(j*25), 100, 140));
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
    m_undo = new QPushButton("Undo", this);
    m_undo->setGeometry(QRect(800, 120, 210, 50));
    m_undo->setFont(boldFont);
    m_undo->setStyleSheet("background-color: transparent; border: none; color: rgb(255, 185, 0); text-align: left; padding-left: 10px;"); 
    m_undo->setVisible(false);
    connect(m_undo, &QPushButton::clicked, this, &SolitaireUI::undoPressed);

    m_newGame = new QPushButton("New Game", this);
    m_newGame->setGeometry(QRect(800, 60, 210, 50));
    m_newGame->setFont(boldFont);
    m_newGame->setStyleSheet("background-color: transparent; border: none; color: rgb(255, 185, 0); text-align: left; padding-left: 10px;"); 
    m_newGame->setVisible(true);
    connect(m_newGame, &QPushButton::clicked, this, &SolitaireUI::newGamePressed);

    m_easy = new QPushButton("easy", this);
    m_easy->setGeometry(QRect(400, 500, 200, 50));
    m_easy->setStyleSheet("background-color: transparent; border: none; color: rgb(255,185,0);");
    m_easy->setFont(boldFont);
    m_easy->setText(QString("Easy Difficulty"));
    m_easy->setVisible(false);
    connect(m_easy, &QPushButton::clicked, this, &SolitaireUI::easyClicked);

    m_startLogo = new QLabel(this);
    m_startLogo->setGeometry(QRect(300,200,480,200));
    m_startLogo->setStyleSheet("background-image: url(" + assetPath + "logo.png); border: none; background-position: center; background_size: contain;");

    m_hard = new QPushButton("hard", this);
    m_hard->setGeometry(QRect(400, 600, 200, 50));
    m_hard->setStyleSheet("background-color: transparent; border: none; color: rgb(255,185,0);");
    m_hard->setFont(boldFont);
    m_hard->setText(QString("Hard Difficulty"));
    m_hard->setVisible(false);
    connect(m_hard, &QPushButton::clicked, this, &SolitaireUI::hardClicked);

    m_moves = new QLabel(this);
    m_moves->setGeometry(QRect(300,930,300,50));
    //lightFont.setPointSize(42);
    m_moves->setStyleSheet("QLabel { color : rgb(255,185,0); }");
    m_moves->setFont(lightFont);
    m_moves->setText(QString("Moves: "));

    m_winMoves = new QLabel(this);
    m_winMoves->setGeometry(QRect(250,450,300,100));
    //m_winMoves->setStyleSheet("QLabel { color : rgb(255,185,0); }");
    m_winMoves->setFont(lightLargeFont);

    m_timer = new QLabel(this);           
    m_timer->setGeometry(QRect(600, 930, 300, 50));
    m_timer->setFont(lightFont);
    m_timer->setStyleSheet("QLabel { color : rgb(255,185,0); }");
    m_timer->setText(QString("Time: "));

    m_winTime = new QLabel(this);
    m_winTime->setGeometry(QRect(600,450,300,100));
    //m_winTime->setStyleSheet("QLabel { color : rgb(255,185,0); }");
    m_winTime->setFont(lightLargeFont);

    m_noMovesLeft = new QLabel(this);
    m_noMovesLeft->setGeometry(QRect(600,740,200,50));
    m_noMovesLeft->setFont(lightFont);
    m_noMovesLeft->setPalette(palette);
    m_noMovesLeft->setText(QString(""));

    int alignment[5][4] = {{150,575,800,100},{200,600,225,400},{425,600,150,400},{575,600,150,400},{725,600,150,400}};
    for(int i=0; i<5; i++)
    {
        m_winScreen[i] = new QLabel(this);
        m_winScreen[i]->setGeometry(QRect(alignment[i][0],alignment[i][1],alignment[i][2],alignment[i][3]));
        m_winScreen[i]->setFont(lightFont);
        if(i<2)
        {m_winScreen[i]->setStyleSheet("QLabel { color : rgb(255,185,0); }");}
        else
        {m_winScreen[i]->setStyleSheet("QLabel { color : rgb(255,255,255); }");}
        m_winScreen[i]->setVisible(false);
        m_winScreen[i]->setAlignment(Qt::AlignCenter);
        m_winScreen[i]->setText(QString());
    }

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        elapsedMilliseconds = elapsedTimer.elapsed();
        qint64 elapsedSeconds = elapsedMilliseconds / 1000;
        qint64 minutes = elapsedSeconds / 60;
        qint64 seconds = elapsedSeconds % 60;
        QString elapsedTimeStr = QString("%1:%2")
        .arg(minutes, 1, 10, QLatin1Char('0'))  
        .arg(seconds, 2, 10, QLatin1Char('0')); 
        m_timer->setText("Time: " + elapsedTimeStr);
    });

    m_pSolitaire = new Solitaire();
    for(int i=0; i<5; i++)
    {
        m_winScreen[i]->setVisible(true);
    }*/
}


ForbiddenIslandUI::~ForbiddenIslandUI()
{
    delete ui;
}


/*void ForbiddenIslandUI::dealCards()
{
    if(gameStarted == true and win == false)
    {
        m_pSolitaire->qFileLoss();
    }
    m_moves->setVisible(true);
    m_timer->setVisible(true);
    m_winMoves->setVisible(false);
    m_winTime->setVisible(false);
    m_pD[0]->setVisible(true);
    m_undo->setVisible(true);
    m_easy->setVisible(false);
    m_hard->setVisible(false);
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
    for(int i=0; i<4; i++)
    {
        m_pA[i]->setVisible(true);
    }
    gameStarted = false;
    timer.stop();
    elapsedMilliseconds = 0;
    m_timer->setText("Time:");
    for(int i=0; i<5; i++)
    {
        m_winScreen[i]->setVisible(false);
        
    }
}

void ForbiddenIslandUI::refreshUpperSection()
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
    m_moves->setText(QString("Moves: ") + QString::number(moves));


void ForbiddenIslandUI::cardClicked()
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
        // *********** if the aceStack is clicked **************
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
        // *********** if the draw pile or deck stack is clicked *************
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

        refreshUpperSection();
        bool aFinish = m_pSolitaire->checkForWin();
        if (aFinish == true){autoFinish();}
        m_pSolitaire->printField();
        bool canPlay = m_pSolitaire->checkCanPlay();
        bool win = false;
        win = m_pSolitaire->checkForWin();
        std::cout << "Can Play = " << canPlay << endl;
        if (canPlay == false)
        {
            if (win == false)
            {
                m_noMovesLeft->setText(QString("No Moves"));
            }
        }
        else
        {
            m_noMovesLeft->setText(QString());
        }
    }
}


void ForbiddenIslandUI::autoFinish()
{
    std::cout << "In autoFinish \n";
    bool gameDone = false;
    bool win = false;
    m_noMovesLeft->setText(QString("Auto Finish!"));
    while (gameDone == false)                       // cycle through all remaining cards and play them to the Aces
    {
        gameDone = m_pSolitaire->nextCard();
        m_pSolitaire->incrementMoves();
        refreshUpperSection();
        delayTimer(200);
    }
    win = m_pSolitaire->checkForWin();
    std::cout << "******* In UI AutoFinish win = " << win << "\n";
    if (win == true) {postWin();}
}


void ForbiddenIslandUI::postWin()
{
    m_undo->setVisible(false);
    m_moves->setVisible(false);
    m_timer->setVisible(false);
    m_winMoves->setVisible(true);
    m_winTime->setVisible(true);
    m_easy->setVisible(false);
    m_hard->setVisible(false);
    timer.stop();
    elapsedMilliseconds = elapsedTimer.elapsed();
    qint64 elapsedSeconds = elapsedMilliseconds / 1000;
    int seconds = static_cast<int>(elapsedMilliseconds / 1000);
    m_pD[0]->setEnabled(false);
    m_pD[0]->setVisible(false);
    m_pD[1]->setEnabled(false);
    m_pD[1]->setIcon(QPixmap()); 
    m_startLogo->setVisible(true);
    std::vector<int> winOutput(11);
    int moves = m_pSolitaire->getMoves();
    //winOutput = m_pSolitaire-> qFileWin(seconds, moves);
    winOutput = m_pSolitaire-> qFileWin(360,200);

    int totalGames = winOutput[0];
    int currentWins = winOutput[1];
    int bestWins = winOutput[2];
    int totalWins = winOutput[3];
    int winRate = winOutput[4];
    qint64 tMinutes = seconds / 60;
    qint64 tSeconds = seconds % 60;
    QString pTime = QString("%1:%2").arg(tMinutes, 1, 10, QLatin1Char('0')).arg(tSeconds, 2, 10, QLatin1Char('0')); ;
    int timeRank = winOutput[5];
    int moveRank = winOutput[6];
    int bestMoves = winOutput[7];
    int bestTime = winOutput[8];
    qint64 bMinutes = bestTime / 60;
    qint64 bSeconds = bestTime % 60;
    QString pBestTime = QString("%1:%2").arg(bMinutes, 1, 10, QLatin1Char('0')).arg(bSeconds, 2, 10, QLatin1Char('0')); 

    QString winText[5] = {
    QString("<font color='#FFB900'>Total Wins: </font><font color='#FFFFFF'> %1 </font> &nbsp;&nbsp;&nbsp;&nbsp;"
            "<font color='#FFB900'> Total Games: </font><font color='#FFFFFF'> %2 </font> &nbsp;&nbsp;&nbsp;&nbsp;"
            "<font color='#FFB900'> Win Percentage: </font><font color='#FFFFFF'> %3% </font>")
            .arg(totalWins)
            .arg(totalGames)
            .arg(winRate),
    QString("\nMoves\nTime\nWin Streak"),
    QString("<font color='#FFB900'> Current </font><br>"
            "<font color='#FFFFFF'> %1 </font><br>"
            "<font color='#FFFFFF'> %2 </font><br>"
            "<font color='#FFFFFF'> %3 </font>")
            .arg(moves)
            .arg(pTime)
            .arg(currentWins),
    QString("<font color='#FFB900'> Rank </font><br>"
            "<font colot='#FFFFFF'> %1 </font><br>"
            "<font color='#FFFFFF'> %2 </font><br<br")
            .arg(moveRank)
            .arg(timeRank),
    QString("<font color='#FFB900'> Best </font><br>"
            "<font color='#FFFFFF'> %1 </font><br>"
            "<font color='#FFFFFF'> %2 </font><br>"
            "<font color='#FFFFFF'> %3 </font>")
        .arg(bestMoves)
        .arg(pBestTime)
        .arg(bestWins)};

    for(int i=0; i<5; i++)
    {
        m_winScreen[i]->setVisible(true);
        m_winScreen[i]->setText(winText[i]);
    }
    m_winMoves->setText("<font color='#FFB900'>Moves: </font>"
                    "<font color='#FFFFFF'>144</font>");
    m_winTime->setText("<font color='#FFB900'>Time: </font>"
                    "<font color='#FFFFFF'>2:15</font>");
    m_noMovesLeft->setText(QString());
}


void ForbiddenIslandUI::enableDrawPile(int pile, int id)              // this method makes a draw pile show up graphically
{
    m_pD[pile]->setEnabled(true);
    m_pD[pile]->setIcon(QPixmap(cardImage[id]));
    m_pD[pile]->raise();
}


void ForbiddenIslandUI::disableDrawPile(int pile)
{
   m_pD[pile]->setEnabled(false);
    m_pD[pile]->setIcon(QPixmap());
    m_pD[pile]->setStyleSheet(QString::fromUtf8("border: none"));
    if (pile>1)
    {
        m_pD[pile-1]->setEnabled(true);
        m_pD[pile]->lower();              // lower the card 
    }
}*/
 

void ForbiddenIslandUI::paintEvent(QPaintEvent *event) 
{
    QPainter painter(this);
    // Paint the green background image
    painter.drawPixmap(0, 0, width(), height(), water);
}


/*void SolitaireUI::delayTimer(int delay)
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

void SolitaireUI::newGamePressed()
{
    std::cout << "New game button pressed\n";
    m_hard->setVisible(true);
    m_hard->raise();
    m_easy->setVisible(true);
    m_easy->raise();
    for(int i=0; i<5; i++)
    {
        m_winScreen[i]->setVisible(false);
    }
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
            int remaining = m_pSolitaire->getDrawPileSize();
            m_pD[0]->setIcon(QPixmap());
            if (remaining > 0)
            {
                m_pD[0]->setText("Flip Deck?");  
            }
            else{
                m_pD[0]->setText("No Cards");
            }
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
    int drawPileSize = m_pSolitaire->getDrawPileSize();     // this is for the draw pile and the flipped cards
    if (deckSize > 0)                                       // This is the main solitaire deck
    {
        m_pD[0]->setEnabled(true);
        m_pD[0]->setIcon(QPixmap(cardImage[0]));
        m_pD[0]->setText(QString());
    }
    else
    {
        m_pD[0]->setIcon(QPixmap());
        if (drawPileSize > 0)
        {
            m_pD[0]->setText("Flip Deck?");  
        } 
        else
        {
            m_pD[0]->setText("No Cards");
        }
    }

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
    m_startLogo->setVisible(false);
    m_pSolitaire->makeWinnableDeck();
    dealCards();
    refreshUpperSection();
    m_noMovesLeft->setText("");
    refreshDecks();
}


void SolitaireUI::hardClicked()
{
    m_startLogo->setVisible(false);
    m_pSolitaire->playGame();               
    dealCards();
    refreshUpperSection();
    m_noMovesLeft->setText("");
    refreshDecks();
}*/
