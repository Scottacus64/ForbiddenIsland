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
    QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/fiLogo.png";
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
    for(int i=0; i<7; i++)
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
        m_floodDecks[i]->setGeometry(QRect(1100 + (i*150), 300, 90, 126));
        m_floodDecks[i]->setIcon(QPixmap(path));
        m_floodDecks[i]->setText(QString());
        m_treasureDecks[i] = new QPushButton(QString::fromStdString("tD") + QString::number(i), this);
        m_treasureDecks[i]->setObjectName(QString::fromStdString("tD") + QString::number(i));
        QString path2 = QCoreApplication::applicationDirPath() + "/../CardPNGs/FIC";
        m_treasureDecks[i]->setGeometry(QRect(1100 + (i*150), 450, 90, 126));
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

    m_wrCard = new QLabel(this);
    path = QCoreApplication::applicationDirPath() + "/../CardPNGs/wrCard.png";
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
            cout << "Number of Players: " << numberOfPlayers << endl;
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
                    transform.rotate(-90);
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
                    m_pGame->drawTreasureCards(p);
                    int tValue = m_pGame->getPlayerTreasureCard(p,i);
                    //string cName = "T" + to_string(tValue);
                    //QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(cName);
                    //QPixmap pixmap(path);
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
            dialog->setText("Choose a Difficulty Level");
            m_dialog[3]->setText("Novice");
            m_dialog[4]->setText("Normal");
            m_dialog[5]->setText("Elite");
            m_dialog[6]->setVisible(true);
            m_dialog[6]->setEnabled(true);
            m_dialog[6]->setText("Legendary");
            dialogMode = 1;
            return;
        }
        if (dialogMode == 1)
        {  
            int difficulty = 0;
            string name = clickedButton->objectName().toStdString();
            if(name == "dB3"){difficulty = 0;}
            if(name == "dB4"){difficulty = 1;}
            if(name == "dB5"){difficulty = 2;}
            if(name == "dB6"){difficulty = 3;}
            m_pGame->setWaterLevel(difficulty);

            dialog->setText("Press button to flood\nsix island tiles");
            m_dialog[3]->setEnabled(false);
            m_dialog[5]->setEnabled(false);
            m_dialog[6]->setEnabled(false);
            m_dialog[3]->setVisible(false);
            m_dialog[5]->setVisible(false);
            m_dialog[6]->setVisible(false);
            QString numLeft = QString::number(squaresToFlood);
            m_dialog[4]->setText(numLeft + " tiles left");
            dialogMode = 2;
            updateWaterRise();
            return;
        }
        if(dialogMode == 2)  // Flood tiles
        {
            int location = m_pGame->flipFlood();
            string iName = m_pGame->getIslandCardName(location);
            string name = iName + ".png";
            string cName = iName;     // copy the original string
            cName.pop_back();         // remove the last character
            cName += "C.png";         // append "C.png"
            cout << cName << endl;
            QString path2 = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(name);
            m_iC[location]->setIcon(QPixmap(path2));

            QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(cName);
            m_treasureDecks[1]->setVisible(true);
            m_treasureDecks[1]->setEnabled(true);
            m_treasureDecks[1]->setIcon(QPixmap(path));
            squaresToFlood -=1;
            QString numLeft = QString::number(squaresToFlood);
            m_dialog[4]->setText(numLeft + " tiles left");
            if(squaresToFlood < 1)
            {
                updateActions();
                int x =0;
                int y=0;
                for(int i=0; i<7; i++)
                {
                    m_dialog[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
                    m_dialog[i]->setVisible(true);
                    m_dialog[i]->setEnabled(true);
                    if(i%3 == 0 && i>0){x+=1; y=0;}
                    m_dialog[i]->setGeometry(QRect(1060+(x*210), 730+(y*70), 200, 60)); 
                    y+=1;
                }
                m_dialog[0]->setText("Move");
                m_dialog[1]->setText("Shore Up");
                m_dialog[2]->setText("Give Treasure");
                m_dialog[3]->setText("Play card");
                m_dialog[4]->setText("Get Treasure");
                m_dialog[5]->setText("Use Special Ability");
                m_dialog[6]->setText("End Turn");
                dialogMode = 3;
                return;
            }
            //clearDialogButtons();
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
                fly == false;
                sendTreasure = false;
                moveOther = false;  
                updateActions();   
                for(int i=0; i<directions.size(); i++)
                {
                    if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
                    {
                        highlightMove(gridLocation+offset[directions[i]]);                 
                    }
                }
            } 
            if (playerAction == 1)          //shore up
            {
                fly == false;
                sendTreasure = false;
                moveOther = false;  
                updateActions();  
                checkForShoreUp();
            }
            if (playerAction == 5)          //speecial ability
            {
                Player* activePlayer = m_pGame->getActivePlayer();  
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
            if (playerAction == 6)          //end turn
            {
                fly == false;
                sendTreasure = false;
                moveOther = false; 
                updateActions();   
                m_pGame->nextPlayer();
                updateActions();
                clearDialogButtons();
            }
        } 

    }
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
    dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nActions Left: " + QString::number(actions));
    if(fly == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a tile to fly there");}
    if(moveOther == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick another player to move");}
    if(sendTreasure == true && playerPicked == false){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a Player to receive");}
    if(sendTreasure == true && playerPicked == true){dialog->setText("Choose an action for: " + QString::fromStdString(pName) + "\nClick a treasure to give");}
}


void ForbiddenIslandUI::clearDialogButtons()
{
    for(int i=0; i<7; i++)
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
                m_dialog[5]->setVisible(false);
                break;    
            case 2:
                m_dialog[1]->setText("Shore Up");
                m_dialog[5]->setVisible(false);
                break;
            case 3:
                m_dialog[1]->setText("Shore Up");
                m_dialog[5]->setVisible(true);
                m_dialog[5]->setText("Fly");
                break;
            case 4:
                m_dialog[1]->setText("Shore Up");
                m_dialog[5]->setVisible(true);
                m_dialog[5]->setText("Move Teammate x2");
                break;
            case 5:
                m_dialog[1]->setText("Shore Up");
                m_dialog[5]->setVisible(false);
                break;
            case 6:
                m_dialog[1]->setText("Shore Up");
                m_dialog[5]->setVisible(true);
                m_dialog[5]->setText("Send Treasure");
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
    if (regex_match(sName, match, re)) 
    {
        playerSlot = stoi(match[1].str());
        gridLocation = stoi(match[2].str());
        cout << "x = " << playerSlot << ", y = " << gridLocation << std::endl;
    }

    if(playerAction == 0)
    {
        vector <int> offset {-6,-5,1,7,6,5,-1,-7};
        vector <int> directions = {0,2,4,6};
        validMoves.clear();
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
            
            // highlight valid locations
        }
        
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
    if (regex_match(sName, match, re))
    {
        iLocation = stoi(match[1].str());
    }
    if(playerAction == 0 || playerAction == 1)
    {
        vector <int> directions = {6,5,-1,-7,-6,-5,1,7};
        int dir;
        Player* activePlayer = m_pGame->getActivePlayer();
        int pLocation = activePlayer->getLocation();
        int delta = pLocation - iLocation;
        for(int i=0; i<8; i++) 
        {
            if(delta == directions[i]){dir = i;}
        }
        
        if (playerAction == 0){m_pGame->movePlayer(*activePlayer, dir);}
        if (playerAction == 1)
        {
            bool eShore;
            if(pLocation == iLocation)
            {eShore = m_pGame->shoreUp(8);}
            else
            {eShore = m_pGame->shoreUp(dir);}
            engineerSU = eShore;
        }
    }
    if(playerAction == 5 && fly == true)
    {
        Player* player = m_pGame->getActivePlayer();
        player->fly(iLocation);
        int actions = player->getActions();
        if(actions < 1){m_pGame->nextPlayer();}
        fly = false;
    }
    updateActions();
    updatePawns();
    clearDialogButtons();
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
    int wl = m_pGame->getWaterLevel();
    m_wrIndicator->setGeometry(1490, wrY[wl], 46, 21);
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


int ForbiddenIslandUI::playerClicked()
{
    QPushButton* playerClicked = qobject_cast<QPushButton*>(sender());
    QString iName = playerClicked->objectName();
    string sName = iName.toStdString();
    receivingPlayer =  sName[1] - '0';
    cout << "Player Clicked: " << receivingPlayer << endl;
    if(sendTreasure ==  true){playerPicked = true;}
    updateActions();
}

int ForbiddenIslandUI::cardClicked()
{
    QPushButton* cardClicked = qobject_cast<QPushButton*>(sender());
    QString iName = cardClicked->objectName();
    string sName = iName.toStdString();
    int cardNumber= sName.back() - '0';   
    cout << "Card number: " << cardNumber << endl;
    m_pGame->sendTreasure(receivingPlayer, cardNumber);
    updateCards();
    if(playerPicked == true)
    {
        sendTreasure = false;
        playerPicked = false;
    }
    updateActions();
    clearDialogButtons();
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

