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
const std::vector<std::string> ForbiddenIslandUI::cardTreasure =    {"WRBC", "EWR", "FWR", "GWR", "HLWR", "LWR",  "SBWR", "WR"};
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
    boldLargeFont.setPointSize(62);

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
            //QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/WRBC";
            //QPixmap pixmap(path);

            //pixmap = pixmap.scaled(90, 126, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if(player == 2){m_playerCards[player][i]->setGeometry(QRect(290 + (i * 100), 5, 90, 126));}
            if(player == 0){m_playerCards[player][i]->setGeometry(QRect(290 + (i * 100), 850, 90, 126));}
            if(player == 1){m_playerCards[player][i]->setGeometry(QRect(20, 640 - (i * 100), 126, 90));}
            if(player == 3){m_playerCards[player][i]->setGeometry(QRect(900, 640 - (i * 100), 126, 90));}
            if (player == 1 || player == 3) 
            {
                QTransform transform;
                transform.rotate(90);
                //pixmap = pixmap.transformed(transform);
                m_playerCards[player][i]->setIconSize(QSize(126,90));
                m_playerCards[player][i]->setStyleSheet("border: none; padding: 0px; margin: 0px;");
            }
            else
            {
                m_playerCards[player][i]->setIconSize(QSize(90, 126));
            }
            //m_playerCards[player][i]->setIcon(pixmap); 
            m_playerCards[player][i]->setText(QString());
            m_playerCards[player][i]->setEnabled(false);
            m_playerCards[player][i]->setVisible(false);
            //connect(m_pA[i], &QPushButton::clicked, this, &ForbiddenIslandUI::cardClicked);
        }
    }
    // set up the flood and treasure draw and discard decks
    for(int i=0; i<2; i++)
    {
        m_floodDecks[i] = new QPushButton(QString::fromStdString("fD") + QString::number(i), this);
        m_floodDecks[i]->setObjectName(QString::fromStdString("fD") + QString::number(i));
        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/WRBC";
        m_floodDecks[i]->setGeometry(QRect(1250 + (i*150), 300, 90, 126));
        m_floodDecks[i]->setIcon(QPixmap(path));
        m_floodDecks[i]->setText(QString());
        m_treasureDecks[i] = new QPushButton(QString::fromStdString("tD") + QString::number(i), this);
        m_treasureDecks[i]->setObjectName(QString::fromStdString("tD") + QString::number(i));
        QString path2 = QCoreApplication::applicationDirPath() + "/../CardPNGs/FIC";
        m_treasureDecks[i]->setGeometry(QRect(1250 + (i*150), 450, 90, 126));
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
            }
            // deal 2 treasure cards to each player
            for(int i=0; i<2; i++)
            {
                for(int p=0; p<numberOfPlayers; p++)
                {
                    m_pGame->drawTreasureCards(p);
                    int tValue =m_pGame->getPlayerTreasureCard(p,i);
                    if(tValue < 7)
                    {
                        string cName = "T" + to_string(tValue);
                        QString path = QCoreApplication::applicationDirPath() + "/../CardPNGs/" + QString::fromStdString(cName);
                        QPixmap pixmap(path);
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
            dialogMode = 1;
            dialog->setText("Press button to flood\nsix island tiles");
            m_dialog[3]->setEnabled(false);
            m_dialog[5]->setEnabled(false);
            m_dialog[3]->setVisible(false);
            m_dialog[5]->setVisible(false);
            QString numLeft = QString::number(squaresToFlood);
            m_dialog[4]->setText(numLeft + " tiles left");
            return;
        }
        if(dialogMode == 1)  // Flood tiles
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
                Player* aPlayer =m_pGame->getActivePlayer();
                string pName = aPlayer->getPlayerName();
                dialog->setText("Choose an action for: " + QString::fromStdString(pName));
                for(int i=0; i<7; i++)
                {
                    m_dialog[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
                    m_dialog[i]->setVisible(true);
                    m_dialog[i]->setEnabled(true);
                }
                m_dialog[0]->setText("Move");
                m_dialog[1]->setText("Shore Up");
                m_dialog[2]->setText("Give Treasure");
                m_dialog[3]->setText("Play card");
                m_dialog[4]->setText("Get Treasure");
                m_dialog[5]->setText("Use Special Ability");
                m_dialog[6]->setText("End Turn");
                dialogMode = 2;
                return;
            }
        }
        if(dialogMode == 2)  // Players Up
        {
            for(int i=0; i<7; i++)
            {
                m_dialog[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
            }
            clickedButton->setStyleSheet("background-color: rgb(234, 196, 146);");
            playerAction = clickedButton->objectName().mid(2).toInt();                  // Set up player actions based upon button pressed
            validMoves.clear();
            vector <int> offset {-6,-5,1,7,6,5,-1,-7};
            vector <int> directions = {0,2,4,6}; 
            Player* activePlayer = m_pGame->getActivePlayer();
            int gridLocation = activePlayer->getLocation();
            if(activePlayer->getPlayerClass() == 2){directions = {0,1,2,3,4,5,6,7};}
            if (playerAction == 0)            // move player
            {    
                updateIsleTiles();      
                for(int i=0; i<directions.size(); i++)
                {
                    if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
                    {
                        highlightMove(gridLocation+offset[directions[i]]);
                        validMoves.push_back(directions[i]);                     
                    }
                }
            } 
            if (playerAction == 1)          //shore up
            {
                updateIsleTiles();
                for(int i=0; i<directions.size(); i++)
                {
                    if (m_pGame->checkValidMove(gridLocation,directions[i]) > 0)
                    {
                        highlightShoreUp(gridLocation+offset[directions[i]]);
                        validMoves.push_back(directions[i]);                     
                    }
                }
            }
        }
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
    updateIsleTiles();
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
