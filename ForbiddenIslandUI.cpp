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
#include <regex>
#include <vector>
// set up all of the card image QPixmaps
const std::vector<std::string> ForbiddenIslandUI::cardTreasure =    {"WRBC", "T1", "T2", "T3", "T4", "T5", "T6", "T7"};
const std::vector<std::string> ForbiddenIslandUI::cardFlood =       {"BBC", "BGC", "CAC", "CEC", "CFC", "CGC", "CPC", "CSC", "DDC", "FLC", "GGC", "HGC", "IGC", "LLC", "MMC", "ObC", "PRC", "SGC", "THC", "TMC", "TPC", "TSC", "WtC", "WGC"};
const std::vector<std::string> ForbiddenIslandUI::cardIsland =      {"BBF", "BGF", "CAF", "CEF", "CFF", "CGF", "CPF", "CSF", "DDF", "FLF", "GGF", "HGF", "IGF", "LLF", "MMF", "ObF", "PRF", "SGF", "THF", "TMF", "TPF", "TSF", "WtF", "WGF"};
const std::vector<std::string> ForbiddenIslandUI::cardIslandFlood = {"BBB", "BGB", "CAB", "CEB", "CFB", "CGB", "CPB", "CSB", "DDB", "FLB", "GGB", "HGB", "IGB", "LLB", "MMB", "ObB", "PRB", "SGB", "THB", "TMB", "TPB", "TSB", "WtB", "WGB"};
const std::vector<std::string> ForbiddenIslandUI::cardPlayer =      {"DC", "EC", "ExC", "MC", "NC", "PC"};
const std::vector<std::string> ForbiddenIslandUI::treasure =        {"TR1", "TR2", "TR3", "TR4"};


ForbiddenIslandUI::~ForbiddenIslandUI()
{
    delete m_pGame;
}


ForbiddenIslandUI::ForbiddenIslandUI(QWidget *parent)
    : QWidget(parent)

{
    m_pGame = new Game();
    QString appDir = QCoreApplication::applicationDirPath();
    QString assetPath = QDir::cleanPath(appDir + QDir::separator() + "CardPngs") + QDir::separator();

    //ui->setupUi(this);
    this->setObjectName("ForbiddenIslandUI");
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int screenWidth = screenGeometry.width();
    this->resize(2100, 1000);
    //this->move(((screenWidth/2) -(2100/2)), 0);

    // set up the FG font
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
    boldLargeFont.setPointSize(55);

    // Load LNLight font
    int lightFontId = QFontDatabase::addApplicationFont(assetPath + "LNLight.ttf");
    QString lightFontFamily;
    lightFontFamily = QFontDatabase::applicationFontFamilies(lightFontId).at(0);
    QFont lightFont(lightFontFamily); 
    lightFont.setPointSize(42);

    QFont lightLargeFont(lightFontFamily); 
    lightLargeFont.setPointSize(84);

    // Create QLabel and QPushButton instances
    logo = new QLabel(this);
    QString path = QCoreApplication::applicationDirPath() + "/CardPNGs/fiLogo.png";
    qDebug() << QCoreApplication::applicationDirPath();
    qDebug() << path;
    QPixmap logoPixmap(path);  
    logo->setPixmap(logoPixmap);
    logo->setScaledContents(true);
    logo->setGeometry(1100, 40, 500, 200);
    logo->raise();  // Ensure it's on top

    dialog = new QLabel(this);
    dialog->setGeometry(1050, 600, 650, 350);
    dialog->setFont(boldLargeFont);
    dialog->setMargin(10);  
    dialog->setStyleSheet("background-color:rgb(96, 158, 219); color: white;"); 
    dialog->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    dialog->setText("How many players?");
    dialog->raise();

    // set up dialog push buttons
    int y = 0;
    int x = 0;
    int num = 2;
    for(int i=0; i<6; i++)
    {
        if(i%3 == 0 && i>0){x+=1; y=0;}
        string name = "dB" + to_string(i);
        m_dialog[i] = new QPushButton(QString::fromStdString(name), this);
        m_dialog[i]->setObjectName(QString::fromStdString(name));
        m_dialog[i]->setGeometry(QRect(1060+(x*210), 700+(y*70), 200, 60));      
        if(i>2 && i<6)
        {
            m_dialog[i]->setText(QString(QString::number(num)));
            m_dialog[i]->setEnabled(true);
            m_dialog[i]->setVisible(true); 
            num+=1;
        }
        else
        {
            m_dialog[i]->setEnabled(false);
            m_dialog[i]->setVisible(false); 
        }
        y+=1;
        connect(m_dialog[i], &QPushButton::clicked, this, &ForbiddenIslandUI::dialogButtonClicked);
    }

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
    for(int i=0; i<cardIslandFlood.size(); i++)
    {
        cardImageIslandFlood[i] = QPixmap(assetPath + QString::fromStdString(cardIslandFlood[i]) + ".png");
    }
    for(int i=0; i<cardPlayer.size(); i++)
    {
        cardImagePlayer[i] = QPixmap(assetPath + QString::fromStdString(cardPlayer[i]) + ".png");
    }
    for(int i=0; i<4; i++)
    {
        treasureImage[i] = QPixmap(assetPath + QString::fromStdString(treasure[i]) + ".png");
    }

    // start up a new game

    m_pGame->newGame();

    // set up the island
    y=0; x=0;
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
        string cardPV = m_pGame->getIslandCardName(i);
        m_iC[i] = new QPushButton(QString::fromStdString(name), this);
        m_iC[i]->setObjectName(QString::fromStdString(name));
        m_iC[i]->setGeometry(QRect(220 + (x*100), 190 + (y*100), 100, 100));
        int slot = 0;
        for(int i=0; i< cardIsland.size(); i++)
        {
            if(cardIsland[i] == cardPV){slot = i;}
        }
        m_iC[i]->setIcon(cardImageIsland[slot]);
        m_iC[i]->setText(QString());
        m_iC[i]->setEnabled(true);
        m_iC[i]->setVisible(true);
        QSize iconSize(100, 100);
        m_iC[i]->setIconSize(iconSize);
        connect(m_iC[i], &QPushButton::clicked, this, &ForbiddenIslandUI::iTileClicked);  
        int xDelta = 0;
        int yDelta = 0;
        for(int j=0; j<4; j++)
        {
            if(j==1 || j==3){xDelta = 40;}
            else{xDelta = 0;}
            if(j>1){yDelta = 40;}
            string pName = "pawn" + to_string(j) + ":" + to_string(i);
            m_pawns[j][i] = new QPushButton(QString::fromStdString(pName), this);
            m_pawns[j][i]->setObjectName(QString::fromStdString(pName));
            m_pawns[j][i]->setGeometry(QRect(230+(x*100)+xDelta, 200+(y*100)+yDelta, 40, 40));
            m_pawns[j][i]->setText(QString());
            QPixmap icon(path);
            m_pawns[j][i]->setIcon(icon);
            m_pawns[j][i]->setIconSize(QSize(40, 40));
            m_pawns[j][i]->setStyleSheet(
            "QPushButton { "
            "background-color: transparent; "
            "border: none; "
            "padding: 0px; "
            "margin: 0px; "
            "}"
            );
            m_pawns[j][i]->setEnabled(false);
            m_pawns[j][i]->setVisible(false);  
            connect(m_pawns[j][i], &QPushButton::clicked, this, &ForbiddenIslandUI::pawnClicked);        
        }   
        x+=1;  
        counter +=1;   

    }

    // set up the four player's and treasure hands    
    for (int player=0; player<4; player++)
    {
        for (int i = 0; i < 6; i++) 
        {
            std::string cardName = "P" + std::to_string(player) + std::to_string(i);
            m_playerCards[player][i] = new QPushButton(QString::fromStdString(cardName), this);
            m_playerCards[player][i]->setObjectName(QString::fromStdString(cardName));
            if(player == 2){m_playerCards[player][i]->setGeometry(QRect(290 + (i * 100), 5, 90, 126));}
            if(player == 0){m_playerCards[player][i]->setGeometry(QRect(290 + (i * 100), 850, 90, 126));}
            if(player == 1){m_playerCards[player][i]->setGeometry(QRect(20, 640 - (i * 100), 126, 90));}
            if(player == 3){m_playerCards[player][i]->setGeometry(QRect(900, 640 - (i * 100), 126, 90));}
            if (player == 1 || player == 3) 
            {
                QTransform transform;
                transform.rotate(90);
                m_playerCards[player][i]->setIconSize(QSize(126,90));
                m_playerCards[player][i]->setStyleSheet("border: none; padding: 0px; margin: 0px;");
            }
            else
            {
                m_playerCards[player][i]->setIconSize(QSize(90, 126));
            }
            m_playerCards[player][i]->setText(QString());
            m_playerCards[player][i]->setEnabled(false);
            m_playerCards[player][i]->setVisible(false);
            connect(m_playerCards[player][i], &QPushButton::clicked, this, &ForbiddenIslandUI::cardClicked);
        }
    }
    // set up the flood and treasure draw and discard decks
    for(int i=0; i<2; i++)
    {
        m_floodDecks[i] = new QPushButton(QString::fromStdString("fD") + QString::number(i), this);
        m_floodDecks[i]->setObjectName(QString::fromStdString("fD") + QString::number(i));
        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/WRBC";
        m_floodDecks[i]->setGeometry(QRect(1100 + (i*110), 300, 90, 126));
        m_floodDecks[i]->setIcon(QPixmap(path));
        m_floodDecks[i]->setText(QString());
        m_treasureDecks[i] = new QPushButton(QString::fromStdString("tD") + QString::number(i), this);
        m_treasureDecks[i]->setObjectName(QString::fromStdString("tD") + QString::number(i));
        QString path2 = QCoreApplication::applicationDirPath() + "/../CardPNGs/FIC";
        m_treasureDecks[i]->setGeometry(QRect(1100 + (i*110), 450, 90, 126));
        m_treasureDecks[i]->setIcon(QPixmap(path2));
        m_treasureDecks[i]->setText(QString());
        if(i==0)
        {
            m_floodDecks[i]->setEnabled(true);
            m_floodDecks[i]->setVisible(true);
            m_treasureDecks[i]->setEnabled(true);
            m_treasureDecks[i]->setVisible(true);
        }
        else
        {
            m_floodDecks[i]->setEnabled(false);
            m_floodDecks[i]->setVisible(false);
            m_treasureDecks[i]->setEnabled(false);
            m_treasureDecks[i]->setVisible(false);
        }
        QSize iconSize(90, 126);
        m_floodDecks[i]->setIconSize(iconSize);
        m_floodDecks[i]->setStyleSheet("border: none; padding: 0px; margin: 0px;");
        m_treasureDecks[i]->setIconSize(iconSize);
        m_treasureDecks[i]->setStyleSheet("border: none; padding: 0px; margin: 0px;");
    }

    for(int i =0 ; i<4; i++)
    {
        m_treasure[i] = new QPushButton(this);
        m_treasure[i]->setObjectName(QString::fromStdString("TR") + QString::number(i));
        QImage image = treasureImage[i].toImage().convertToFormat(QImage::Format_ARGB32);
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.fillRect(image.rect(), QColor(0, 0, 0, 102));  // 102 = ~40% alpha
        painter.end();

        QPixmap transparentPixmap = QPixmap::fromImage(image);

        m_treasure[i]->setIcon(transparentPixmap);
        m_treasure[i]->setGeometry(1370, 270 + (i*80), 60, 60);
        m_treasure[i]->setIconSize(m_treasure[i]->size());
        m_treasure[i]->setStyleSheet("border: none; background-color: transparent;");
        m_treasure[i]->raise();  // Ensure it's on top
        connect(m_treasure[i], &QPushButton::clicked, this, &ForbiddenIslandUI::treasureClicked);
    }

    m_wrCard = new QLabel(this);
    path = QCoreApplication::applicationDirPath() + "/CardPNGs/wrCard.png";
    QPixmap wrcPixmap(path);  
    m_wrCard->setPixmap(wrcPixmap);
    m_wrCard->setScaledContents(true);
    m_wrCard->setGeometry(1500, 270, 116, 319);
    m_wrCard->raise();  // Ensure it's on top

    m_wrIndicator = new QLabel(this);
    path = QCoreApplication::applicationDirPath() + "/../CardPNGs/wrIndicator.png";
    QPixmap wriPixmap(path);  
    m_wrIndicator->setPixmap(wriPixmap);
    m_wrIndicator->setScaledContents(true);
    m_wrIndicator->setGeometry(1490, 522, 46, 21);
    m_wrIndicator->raise();  // Ensure it's on top

}


void ForbiddenIslandUI::dialogButtonClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) 
    {
        if(dialogMode == 0)  // Set up UI
        {
            int numberOfPlayers = 2;
            string name = clickedButton->objectName().toStdString();
            cout << name << endl;
            if(name == "dB4"){numberOfPlayers = 3;}
            if(name == "dB5"){numberOfPlayers = 4;}
            m_pGame->createPlayers(numberOfPlayers);
            updatePawns();
            // set up the four player cards
            for(int i=0; i<4; i++)
            {
                QPixmap pixmap;
                std::string cardName = "P" + std::to_string(i);
                Player* pPlayer = m_pGame->getPlayer(i);
                m_player[i] = new QPushButton(QString::fromStdString(cardName), this);
                m_player[i]->setObjectName(QString::fromStdString(cardName));
                if(pPlayer){
                    int playerClass = pPlayer->getPlayerClass();
                    QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/C" + QString::number(playerClass) + ".png";
                    pixmap.load(path);
                    m_player[i]->setEnabled(true);
                    m_player[i]->setVisible(true);
                }
                else
                {
                    m_player[i]->setEnabled(false);
                    m_player[i]->setVisible(true);
                }
                if(i == 2){m_player[i]->setGeometry(QRect(190, 5, 90, 126));}
                if(i == 0){m_player[i]->setGeometry(QRect(190, 850, 90, 126));}
                if(i == 1){m_player[i]->setGeometry(QRect(20, 740, 126, 90));}
                if(i == 3){m_player[i]->setGeometry(QRect(900, 740, 126, 90));}
                if (i == 1 || i == 3) 
                {
                    QTransform transform;
                    transform.rotate(90);
                    pixmap = pixmap.transformed(transform);
                    QSize iconSize (126, 90);
                    m_player[i]->setIconSize(iconSize);
                    m_player[i]->setStyleSheet("border: none; padding: 0px; margin: 0px;");
                }
                else
                {
                    QSize iconSize(90, 126);
                    m_player[i]->setIconSize(iconSize);
                    m_player[i]->setStyleSheet("border: none; padding: 0px; margin: 0px;");
                }
                m_player[i]->setIcon(pixmap); 
                m_player[i]->setText(QString());   
                connect(m_player[i], &QPushButton::clicked, this, &ForbiddenIslandUI::playerClicked);  
            }
            // deal 2 treasure cards to each player
            for(int i=0; i<2; i++)
            {
                for(int p=0; p<numberOfPlayers; p++)
                {
                    bool wrDrawn = m_pGame->drawTreasureCards(p);
                    int tValue = m_pGame->getPlayerTreasureCard(p,i);
                    cout << "Player" << p << " tValue: " << tValue  << endl;
                    QPixmap pixmap = cardImageTreasure[tValue];
                    if(p==1 || p==3)
                    {
                        QTransform transform;
                        transform.rotate(90);
                        pixmap = pixmap.transformed(transform);
                    }
                    m_playerCards[p][i]->setIcon(pixmap); 
                    m_playerCards[p][i]->setText(QString());
                    m_playerCards[p][i]->setEnabled(true);
                    m_playerCards[p][i]->setVisible(true);
                }
            }
            int x=0;
            int y=0;
            for(int i=0; i<6; i++)
            {
                if(i%3 == 0 && i>0){x+=1; y=0;}
                m_dialog[i]->setVisible(true);
                m_dialog[i]->setEnabled(true);
                m_dialog[i]->setGeometry(QRect(1160+(x*210), 700+(y*70), 200, 60)); 
                y+=1;  
            }
            dialog->setText("Choose a Difficulty Level");
            m_dialog[0]->setText("Novice");
            m_dialog[1]->setText("Normal");
            m_dialog[2]->setText("Elite");
            m_dialog[3]->setText("Legendary");
            m_dialog[4]->setEnabled(false);
            m_dialog[4]->setVisible(false);
            m_dialog[5]->setEnabled(false);
            m_dialog[5]->setVisible(false);
            dialogMode = 1;
            return;
        }

        if (dialogMode == 1)
        {  
            int difficulty = 0;
            string name = clickedButton->objectName().toStdString();
            if(name == "dB0"){difficulty = 0;}
            if(name == "dB1"){difficulty = 1;}
            if(name == "dB2"){difficulty = 2;}
            if(name == "dB3"){difficulty = 3;}
            m_pGame->setWaterLevel(difficulty);

            dialog->setText("Press button to flood\nsix island tiles");
            for(int i=0; i<6; i++)
            {
                m_dialog[i]->setEnabled(false);
                m_dialog[i]->setVisible(false);
            }
            m_dialog[1]->setEnabled(true);
            m_dialog[1]->setVisible(true);
            QString numLeft = QString::number(squaresToFlood);
            m_dialog[1]->setGeometry(1300,750,150,60);
            m_dialog[1]->setText(numLeft + " tiles left");
            dialogMode = 2;
            updateWaterRise();
            return;
        }

        if(dialogMode == 2)  // Flood tiles
        {
            floodTiles();
            squaresToFlood -=1;
            QString numLeft = QString::number(squaresToFlood);
            m_dialog[1]->setText(numLeft + " tiles left");
            if(squaresToFlood < 1)
            {
                updateActions();
                setDMode3();
                dialogMode = 3;
                return;
            }
        }

        if(dialogMode == 3)  // Players Up
        {
            clearDialogButtons();
            clickedButton->setStyleSheet("background-color: rgb(234, 196, 146);");      
            playerAction = clickedButton->objectName().mid(2).toInt();                  // Set up player actions based upon button pressed
            cout << playerAction << endl;
            vector <int> offset {-6,-5,1,7,6,5,-1,-7};
            vector <int> directions = {0,2,4,6}; 
            Player* activePlayer = m_pGame->getActivePlayer();
            int gridLocation = activePlayer->getLocation();
            if(activePlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
            updateIsleTiles();
            if (playerAction == 0)            // move player
            {   
                fly = false;
                sendTreasure = false;
                moveOther = false;  
                updateActions();   
                for(int i=0; i<directions.size(); i++)
                {
                    if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
                    {
                        int pSlot = m_pGame->getActivePlayerSlot();
                        highlightMove(gridLocation+offset[directions[i]]);                 
                    }
                }
            } 
            if (playerAction == 1)          //shore up
            {
                fly = false;
                sendTreasure = false;
                moveOther = false;  
                updateActions();  
                checkForShoreUp();
            }
            if(playerAction == 2)           //give treasure
            {
                sameTile.clear();
                for(int i=0; i<m_pGame->getNumberOfPlayers(); i++)
                {
                    Player* tPlayer = m_pGame->getPlayer(i);
                    Player* activePlayer = m_pGame->getActivePlayer();
                    if(tPlayer->getLocation() == activePlayer->getLocation())
                    {
                        sameTile.push_back(i);
                    }
                }
                if(sameTile.size() > 1)
                {
                    giveTreasure = true;
                    updateActions();
                }
                else
                {
                    clearDialogButtons();
                }
            }
            if(playerAction == 3)           //get treasure
            {
                vector <int> pHand;
                getTreasure = false;
                for(int i=0; i<activePlayer->getHandSize(); i++)
                {
                    int cValue = activePlayer->getCardTreasureValue(i);
                    pHand.push_back(cValue);
                }
                std::unordered_map<int, int> counts;
                for (int val : pHand) 
                {
                    counts[val]++;
                    if (counts[val] >= 4) 
                    {
                        getTreasure = true;
                        treasureValue = val;
                    }
                }    
                updateActions();
            }
            if (playerAction == 4)          //speecial ability
            {
                int pClass = activePlayer->getPlayerClass();
                if (pClass == 3)
                {
                    fly = true;
                    updateActions();
                }
                if(pClass == 4)
                {
                    moveOther = true;
                    updateActions();
                }
                if(pClass == 6)
                {
                    sendTreasure = true;
                    updateActions();
                }
            }
            if (playerAction == 5)          //end turn
            {
                fly = false;
                sendTreasure = false;
                moveOther = false; 
                updateActions();   
                clearDialogButtons();
                m_pGame->nextPlayer();
                dialogMode = 4;
                setDMode4();
            }
            return;
        } 

        if(dialogMode == 4)                             // draw cards
        {
            int apSlot = m_pGame->getActivePlayerSlot();

            int handSize = m_pGame->getPlayerHandSize(apSlot);
            if(waterRise == false)
            {
                bool wrDrawn = m_pGame->drawTreasureCards(apSlot);
                updateCards();
                if (wrDrawn == true)
                {
                    
                    m_playerCards[apSlot][handSize]->setVisible(true);
                    m_playerCards[apSlot][handSize]->setEnabled(true);
                    m_playerCards[apSlot][handSize]->setIcon(cardImageTreasure[7]);
                    if(apSlot == 1 || apSlot == 3)
                    {
                        QTransform transform;
                        transform.rotate(90);
                        QPixmap pixmap = cardImageTreasure[7];
                        pixmap = pixmap.transformed(transform);
                        QSize iconSize (126, 90);   
                        m_playerCards[apSlot][handSize]->setIcon(pixmap);      
                    }
                    waterRise = true;
                    m_dialog[1]->setText("Raise Water"); 
                }
                else
                {
                    updateIsleTiles();
                    cardsDrawn +=1;
                    wrDrawn = false;
                }
            }
            else
            {
                floodTiles();
                cardsDrawn +=1;
                m_playerCards[apSlot][handSize]->setVisible(false);
                m_playerCards[apSlot][handSize]->setEnabled(false);
                updateWaterRise();
                updateIsleTiles();
                updateActions();
                waterRise = false;   
                m_dialog[1]->setText("Draw a Card");
            }          
            if(cardsDrawn > 1)
            {
                m_pGame->nextPlayer();
                cardsDrawn = 0;
                dialogMode = 5;
                updateWaterRise();
                setDMode5();
            }
            updateActions();
            return;
        }

        if(dialogMode == 5)                                 // flood tiles
        {
            dialog->setText("Flood " + QString::number(numberOfFloods) + " Island Tiles");
            floodTiles();
            numberOfFloods -=1;
            if(numberOfFloods < 1)
            {
                updateWaterRise();
                dialogMode = 3;
                setDMode3();
            }
        }
    }
}


void ForbiddenIslandUI::floodTiles()
{
    int location = m_pGame->flipFlood();
    cout << "location: " << location << endl;
    string iName = m_pGame->getIslandCardName(location);
    cout << "iName: " << iName << endl;
    bool state0 = false;
    if(iName.length()<3){state0 = true;}
    if(state0 == true)
    {
        string cName = iName + "C.png";
        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(cName);
        m_treasureDecks[1]->setIcon(QPixmap(path));
        m_iC[location]->setVisible(false);
    }
    else
    {
        string name = iName + ".png";
        cout << "Name: " << name << endl;
        string cName = iName;     // copy the original string
        cName.pop_back();         // remove the last character
        cName += "C.png";         // append "C.png"
        cout << "cName: " << cName << endl;
        QString path2 = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(name);
        m_iC[location]->setIcon(QPixmap(path2));
        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(cName);
        m_treasureDecks[1]->setIcon(QPixmap(path));
    }
    m_treasureDecks[1]->setVisible(true);
    m_treasureDecks[1]->setEnabled(true);
}


void ForbiddenIslandUI::checkForShoreUp()
{
    Player* activePlayer = m_pGame->getActivePlayer();
    int gridLocation = activePlayer->getLocation();
    vector <int> offset {-6,-5,1,7,6,5,-1,-7};
    vector <int> directions = {0,2,4,6}; 
    if(activePlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
    for(int i=0; i<directions.size(); i++)
    {
        int location = gridLocation+offset[directions[i]];
        if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0 && m_pGame->getIslandCardFlood(location) == 1)
        {
            highlightShoreUp(gridLocation+offset[directions[i]]);                   
        }
    }
    if (m_pGame->checkValidMove(gridLocation,8) > 0 && m_pGame->getIslandCardFlood(gridLocation) == 1)
    {
    highlightShoreUp(gridLocation);
    }
}


void ForbiddenIslandUI::updateActions()
{
    Player* player = m_pGame->getActivePlayer();
    string pName = player->getPlayerName();
    int actions = player->getActions();
    if(dialogMode == 3){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nActions Left: " + QString::number(actions));}
    if(fly == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a tile to fly there");}
    if(moveOther == true && firstMove == false){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick another player to move");}
    if(moveOther == true && firstMove == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nMove the player again");}
    if(sendTreasure == true && playerPicked == false){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a Player to receive");}
    if(sendTreasure == true && playerPicked == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a treasure to give");}
    if(giveTreasure == true && playerPicked == false){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a Player to receive");}
    if(giveTreasure == true && playerPicked == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a treasure to give");}
    if(getTreasure == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nChoose a treasure to get");}
    if(helo == true && heloFrom < 1){dialog->setText("Choose a location to Helocopter From");}
    if(helo == true && heloFrom > 0){dialog->setText("Choose players to Helocopter\nThen select a destination");}
    if(sandBag == true){dialog->setText("Choose a location to Sandbag");}
    if(dialogMode == 4 && waterRise == false){dialog->setText("Draw a Card");}
    if(dialogMode == 4 && waterRise == true){dialog->setText("Water Rises!");} 
}


void ForbiddenIslandUI::clearDialogButtons()
{
    for(int i=0; i<6; i++)
    {
        m_dialog[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
    }
    int pClass = m_pGame->getActivePlayer()->getPlayerClass();
    switch (pClass)
            {
            case 1:
                if (engineerSU == false)
                { m_dialog[1]->setText("Shore Up x 2");}
                else
                {
                    m_dialog[1]->setText("Shore Up x 1");
                    checkForShoreUp();
                    m_dialog[1]->setStyleSheet("background-color: rgb(234, 196, 146);");
                }
                m_dialog[4]->setVisible(false);
                break;    
            case 2:
                m_dialog[1]->setText("Shore Up");
                m_dialog[4]->setVisible(false);
                break;
            case 3:
                m_dialog[1]->setText("Shore Up");
                m_dialog[4]->setVisible(true);
                m_dialog[4]->setText("Fly");
                break;
            case 4:
                m_dialog[1]->setText("Shore Up");
                m_dialog[4]->setVisible(true);
                m_dialog[4]->setText("Move Teammate x2");
                break;
            case 5:
                m_dialog[1]->setText("Shore Up");
                m_dialog[4]->setVisible(false);
                break;
            case 6:
                m_dialog[1]->setText("Shore Up");
                m_dialog[4]->setVisible(true);
                m_dialog[4]->setText("Send Treasure");
                break;
            }
}


void ForbiddenIslandUI::pawnClicked()
{
    QPushButton* clickedPawn = qobject_cast<QPushButton*>(sender());
    QString pName = clickedPawn->objectName();
    string sName = pName.toStdString();
    regex re(R"(pawn(\d+):(\d+))");
    smatch match;
    int playerSlot;
    int gridLocation;
    vector <int> offset {-6,-5,1,7,6,5,-1,-7};
    vector <int> directions = {0,2,4,6};
    if (regex_match(sName, match, re)) 
    {
        playerSlot = stoi(match[1].str());
        gridLocation = stoi(match[2].str());
        cout << "x = " << playerSlot << ", y = " << gridLocation << std::endl;
    }

    if(playerAction == 0 && heloPlayers.size() == 0)
    {   
        validMoves.clear();
        if(moveOther == false)
        {
            if(playerSlot == m_pGame->getActivePlayerSlot())
            {
                cout << "Active Player found!!" << endl;
                Player* activePlayer = m_pGame->getActivePlayer();
                if(activePlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
                for(int i=0; i<directions.size(); i++)
                {
                    if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
                    {
                        highlightMove(gridLocation+offset[directions[i]]);
                        validMoves.push_back(directions[i]);
                        
                    }
                }
            }
        }
    }

    if(moveOther == true)
    {
        Player* movingPlayer = m_pGame->getPlayer(playerSlot);
        mSlot = playerSlot;
        cout << "Player slot: " << playerSlot << endl;
        if(movingPlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
        for(int i=0; i<directions.size(); i++)
        {
            if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
            {
                highlightMove(gridLocation+offset[directions[i]]);
            }
        }
    }

    if(sendTreasure == true)
    {
        receivingPlayer =  playerSlot;
        cout << "Player Clicked: " << receivingPlayer << endl;
        if(sendTreasure ==  true){playerPicked = true;}
        updateActions();
    }

    if(heloPlayers.size() > 0)
    {
        for(int i=0; i<heloPlayers.size(); i++)
        {
            if(playerSlot == heloPlayers[i])
            {
                bool dup = false;
                for(int j=0; j<heloGroup.size(); j++)
                {
                    if(heloGroup[j] == playerSlot)
                    {dup = true;}
                }
                if(dup == false)
                {heloGroup.push_back(playerSlot);}
            }
        }
        cout << "Players to helo = " ;
        for(int i=0; i<heloGroup.size(); i++)
        {
            cout << heloGroup[i] << "," ;
        }
        cout << endl;
    }
}


void ForbiddenIslandUI::iTileClicked()
{
    QPushButton* clickedIsleTile = qobject_cast<QPushButton*>(sender());
    cout << "Isle Tile clicked" << endl;
    QString iName = clickedIsleTile->objectName();
    string sName = iName.toStdString();
    regex re(R"(iC(\d+))");
    smatch match;
    int iLocation;
    vector <int> directions = {6,5,-1,-7,-6,-5,1,7};
    int dir;
    Player* activePlayer = m_pGame->getActivePlayer();
    if (regex_match(sName, match, re))
    {
        iLocation = stoi(match[1].str());
    }
    if(playerAction == 0 || playerAction == 1)
    {

        int pLocation = activePlayer->getLocation();
        int delta = pLocation - iLocation;
        for(int i=0; i<8; i++) 
        {
            if(delta == directions[i]){dir = i;}
        }
        
        if (playerAction == 0)
        {
            m_pGame->movePlayer(*activePlayer, dir);

        }
        if (playerAction == 1)
        {
            bool eShore;
            if(pLocation == iLocation)
            {
                eShore = m_pGame->shoreUp(8);
                return;
            }
            else
            {eShore = m_pGame->shoreUp(dir);}
            engineerSU = eShore;
        }
            bool nextUp = m_pGame->getNextUp();
            if (nextUp == true)
            {
                dialogMode = 4;
                setDMode4();
            }
    }

    if (playerAction == 4 && moveOther == true)
    {
        cout << "FirstMove: " << firstMove << endl;
        Player* movingPlayer = m_pGame->getPlayer(mSlot);
        int mpLocation = movingPlayer->getLocation();
        int mdelta = mpLocation - iLocation;
        for(int i=0; i<8; i++) 
        {
            if(mdelta == directions[i]){dir = i;}
        }  
        m_pGame->movePlayer(*movingPlayer, dir);
        int loc = movingPlayer->getLocation();

        vector <int> offset {-6,-5,1,7,6,5,-1,-7};
        vector <int> directions = {0,2,4,6};
        if(movingPlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
        updateIsleTiles();
        updatePawns();
        updateActions();
        for(int i=0; i<directions.size(); i++)
        {
            if (m_pGame->checkValidMove(loc,directions[i]) > 0)
            {
                highlightMove(loc+offset[directions[i]]);
            }
        }
        updateActions();
        if(firstMove == true)
        {
            firstMove =! firstMove;
            activePlayer->setActions(-1);
            bool nextUp = m_pGame->getNextUp();
            if (nextUp == true)
            {
                dialogMode = 4;
                setDMode4();
            }
        }
        else
        {  
            firstMove =! firstMove;
            return;
        }
        firstMove =! firstMove;
        

    }
   
    if(playerAction == 4 && fly == true)
    {
        Player* activePlayer = m_pGame->getActivePlayer();
        activePlayer->fly(iLocation, false);
        activePlayer->setActions(-1);
        bool nextUp = m_pGame->getNextUp();
        if (nextUp == true)             //the game class cannot change dialog modes, it sets nextUp which can be gotten by the UI to know if the turn is ended
        {
            dialogMode = 4;
            setDMode4();
        }
        fly = false;
    }

    if(helo == true && heloFrom < 1)
    {
        heloPlayers.clear();
        Player* player;
        for(int i=0; i<m_pGame->getNumberOfPlayers(); i++)
        {
            player = m_pGame->getPlayer(i);
            int pLocation = player->getLocation();
            if(pLocation == iLocation)
            {
                heloPlayers.push_back(i);
            }
        }
        if(heloPlayers.size() > 0){heloFrom = iLocation;}
        updateActions();
    } 

    if(helo == true && heloGroup.size() > 0)
    {
        for(int i=0; i<heloGroup.size(); i++)
        {
            Player* player = m_pGame->getPlayer(heloGroup[i]);
            player->fly(iLocation, true);
        }
        m_pGame->helo(heloPlayer, heloSlot);
        heloGroup.clear();
        helo = false;
        updateCards();
        updateActions();
    }

    if(sandBag == true)
    {
        int iState = m_pGame->getIslandCardFlood(iLocation);
        if(iState == 1)
        {
            m_pGame->sandBag(sandBagPlayer, sandBagSlot, iLocation);
            sandBag = false;
        }
        else
        {
            sandBag = false;
        }
        updateCards();
        updateIsleTiles();
        updateActions();
    }

    updateActions();
    updatePawns();
   // clearDialogButtons();
    updateActions();
    if(engineerSU == true)
    {
        updateIsleTiles();
        checkForShoreUp();
    }
    else{updateIsleTiles();}
}


void ForbiddenIslandUI::updatePawns()
{
    int numberOfPlayers =m_pGame->getNumberOfPlayers();  
    for(int i=0; i<24; i++)
    {
        for(int j=0; j<4; j++)
        {
            m_pawns[j][validSquares[i]]->setIcon(QPixmap());
            m_pawns[j][validSquares[i]]->setEnabled(false);
            m_pawns[j][validSquares[i]]->setVisible(false);
        }
    } 
    for(int i=0; i<numberOfPlayers; i++)
    {
        Player* player = m_pGame->getPlayer(i);
        int loc = player->getLocation();
        int playerClass = player->getPlayerClass();
        cout << i << ";" << playerClass << ":" << loc << endl;
        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/P" + QString::number(playerClass) + ".png";
        QPixmap pixmap3;
        pixmap3.load(path);
        m_pawns[i][loc]->setIcon(pixmap3);
        m_pawns[i][loc]->setVisible(true);
        m_pawns[i][loc]->setEnabled(true);
    }
}


void ForbiddenIslandUI::highlightMove(int tileLocation)
{
    cout << "in HL" << endl;
    QIcon icon = m_iC[tileLocation]->icon();
    QSize size = m_iC[tileLocation]->iconSize(); 
    QPixmap currentPixmap = icon.pixmap(size);
    QPixmap highlightedPixmap = currentPixmap;
    QPainter painter(&highlightedPixmap);
    painter.fillRect(highlightedPixmap.rect(), QColor(255, 255, 0, 100));
    painter.end();
    m_iC[tileLocation]->setIcon(highlightedPixmap);
}


void ForbiddenIslandUI::highlightShoreUp(int tileLocation)
{
    QIcon icon = m_iC[tileLocation]->icon();
    QSize size = m_iC[tileLocation]->iconSize(); 
    QPixmap currentPixmap = icon.pixmap(size);
    QPixmap highlightedPixmap = currentPixmap;
    QPainter painter(&highlightedPixmap);
    painter.fillRect(highlightedPixmap.rect(), QColor(0, 255, 0, 100));
    painter.end();
    m_iC[tileLocation]->setIcon(highlightedPixmap);
}


void ForbiddenIslandUI::updateIsleTiles() 
{
    string cardName;
    int slot = 0;
    for(int i=0; i<36; i++)
    {
        if (std::find(invalidSquares.begin(), invalidSquares.end(), i) != invalidSquares.end()) {continue;}
        cardName = m_pGame->getIslandCardName(i);
        if(cardName != "")
        {
            int cSlot = 0;
            for(int j=0; j<24; j++)
            {
                if(cardIsland[j] == cardName){m_iC[i]->setIcon(cardImageIsland[j]);}
                if(cardIslandFlood[j] == cardName){m_iC[i]->setIcon(cardImageIslandFlood[j]);}
            }
        }
        else
        {
            m_iC[slot]->setIcon(QPixmap());
        }
        slot +=1;
    }
}


void ForbiddenIslandUI::updateWaterRise()
{
    vector <int> wrY = {522,495,470,444,418,390,364,339,314,288};
    vector <int> floods = {2,2,3,3,3,4,4,5,5,5};
    int wl = m_pGame->getWaterLevel();
    m_wrIndicator->setGeometry(1490, wrY[wl], 46, 21);
    numberOfFloods = floods[wl];
}


void ForbiddenIslandUI::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#002366")); 
    QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/water.jpg";
    water = QPixmap(path);
    QRect waterArea(180, 150, 680, 680); 
    painter.drawPixmap(waterArea, water.scaled(waterArea.size(), Qt::KeepAspectRatioByExpanding));
}


void ForbiddenIslandUI::playerClicked()
{
    QPushButton* clickedPlayer = qobject_cast<QPushButton*>(sender());
    QString pName = clickedPlayer->objectName();
    string sName = pName.toStdString();
    if(sendTreasure == true)
    {
        receivingPlayer = sName[1] - '0';
        if(sendTreasure ==  true){playerPicked = true;}
        updateActions();
    }
    if(giveTreasure == true)
    {
        receivingPlayer = sName[1] - '0';
        bool pHere = false;
        for (int i=0; i<sameTile.size(); i++)
        {
            cout << sameTile[i] << ":" << receivingPlayer << endl;
            if(sameTile[i] == receivingPlayer)
            {
                pHere = true;
            }
        }
        if(giveTreasure ==  true && pHere == true){playerPicked = true;}
        updateActions();
    }
    if(moveOther == true)
    {
        vector <int> directions = {0,2,4,6};
        vector <int> offset {-6,-5,1,7,6,5,-1,-7};           
        mSlot = sName[1] - '0';
        Player* movingPlayer = m_pGame->getPlayer(mSlot);
        int gridLocation = movingPlayer->getLocation();
        if(movingPlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
        for(int i=0; i<directions.size(); i++)
        {
            if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
            {
                highlightMove(gridLocation+offset[directions[i]]);
            }
        }
    }
}


void ForbiddenIslandUI::cardClicked()
{
    QPushButton* cardClicked = qobject_cast<QPushButton*>(sender());
    QString iName = cardClicked->objectName();
    string sName = iName.toStdString();
    int slot = sName.back() - '0';
    int player =  sName[1] - '0';;
    cout << sName << " Player: " << player << " Slot: " << slot << endl;
    int cardValue = m_pGame->getPlayerTreasureCard(player, slot);
    if(cardValue == 5)
    {
        heloPlayer = player;
        heloSlot = slot;
        helo = true;
    }
    if(cardValue == 6)
    {
        sandBag = true;
        sandBagPlayer = player;
        sandBagSlot = slot;
    }
    updateActions();
    if(playerPicked == false){return;} 
    m_pGame->sendTreasure(receivingPlayer, slot);
    updateCards();
    if(playerPicked == true)
    {
        giveTreasure = false;
        sendTreasure = false;
        playerPicked = false;
    }
    updateActions();
    receivingPlayer = 9;
    Player* activePlayer = m_pGame->getActivePlayer();
    activePlayer->setActions(-1);
    bool nextUp = m_pGame->getNextUp();
    if (nextUp == true)
    {
        dialogMode = 4;
        setDMode4();
    }
}


void ForbiddenIslandUI::updateCards()
{
    int numberOfPlayers = m_pGame->getNumberOfPlayers();
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<6; j++)
        {
            m_playerCards[i][j]->setVisible(false);
            m_playerCards[i][j]->setEnabled(false);
        }
    }
    for(int p=0; p<numberOfPlayers; p++)
    {
        int hSize = m_pGame->getPlayerHandSize(p);
        for(int i=0; i<hSize; i++)
            {     
                int tValue = m_pGame->getPlayerTreasureCard(p,i);
                //string cName = "T" + to_string(tValue);
                //QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(cName);
                //QPixmap pixmap(path);
                QPixmap pixmap = cardImageTreasure[tValue];
                if(p==1 || p==3)
                {
                    QTransform transform;
                    transform.rotate(90);
                    pixmap = pixmap.transformed(transform);
                }
                m_playerCards[p][i]->setIcon(pixmap); 
                m_playerCards[p][i]->setText(QString());
                m_playerCards[p][i]->setEnabled(true);
                m_playerCards[p][i]->setVisible(true);
        }
            
    }
}


void ForbiddenIslandUI::treasureClicked()
{
    QPushButton* clickedTreasure = qobject_cast<QPushButton*>(sender());
    string name = clickedTreasure->objectName().toStdString();
    int tNumber= name.back() - '0';  
    if (tNumber+1 == treasureValue)
    {
        Player* activePlayer = m_pGame->getActivePlayer();
        m_pGame->getTreasure(*activePlayer, treasureValue);
        updateCards();
        activePlayer->setActions(-1);
        bool nextUp = m_pGame->getNextUp();
        if (nextUp == true)
        {
            dialogMode = 4;
            setDMode4();
        }
        m_treasure[tNumber]->setIcon(treasureImage[tNumber]);
    }

}


void ForbiddenIslandUI::setDMode3()
{
    Player* player = m_pGame->getActivePlayer();
    string pName = player->getPlayerName();
    int actions = player->getActions();
    int x =0;
    int y=0;
    for(int i=0; i<6; i++)
    {
        m_dialog[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
        m_dialog[i]->setVisible(true);
        m_dialog[i]->setEnabled(true);
        if(i%3 == 0 && i>0){x+=1; y=0;}
        m_dialog[i]->setGeometry(QRect(1170+(x*210), 730+(y*70), 200, 60)); 
        y+=1;
    }
    m_dialog[0]->setText("Move");
    m_dialog[1]->setText("Shore Up");
    m_dialog[2]->setText("Give Treasure");
    m_dialog[3]->setText("Get Treasure");
    m_dialog[4]->setText("Use Special Ability");
    m_dialog[5]->setText("End Turn");
    dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nActions Left: " + QString::number(actions));
    //clearDialogButtons();
    updateActions();
}


void ForbiddenIslandUI::setDMode4()
{
    for(int i=0; i<6; i++)
    {
        m_dialog[i]->setEnabled(false);
        m_dialog[i]->setVisible(false);
    }
    m_dialog[1]->setGeometry(1300,750,150,60);
    m_dialog[1]->setText("Draw a Card");
    m_dialog[1]->setVisible(true);
    m_dialog[1]->setEnabled(true);
    dialog->setText("Draw Two Cards");
}


void ForbiddenIslandUI::setDMode5()
{
    dialog->setText("Flood " + QString::number(numberOfFloods) + " Island Tiles");
    m_dialog[1]->setText("Press to Flood");
}
