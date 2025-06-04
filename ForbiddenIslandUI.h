#ifndef FORBIDDENISLANDUI_H
#define FORBIDDENISLANDUI_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include "Game.h"
#include <string>
#include <iostream>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class ForbiddenIslandUI; }
QT_END_NAMESPACE

using namespace std;

class ForbiddenIslandUI : public QWidget
{
    Q_OBJECT


public:
    ForbiddenIslandUI(QWidget *parent = nullptr);
    ~ForbiddenIslandUI();
    void paintEvent(QPaintEvent *event);
    void updatePawns();
    void highlightMove(int tileLocation);
    void highlightShoreUp(int tileLocation);
    void updateIsleTiles();
    void clearDialogButtons();
    void updateActions();
    void updateWaterRise();
    void checkForShoreUp();
    void cardClicked();
    void updateCards();
    void playerClicked();
    void treasureClicked();
    void setDMode3();
    void setDMode4();
    void floodTiles();
 
    
public slots:
    void dialogButtonClicked();
    void pawnClicked();
    void iTileClicked();

private:
    Ui::ForbiddenIslandUI *ui;
    static const vector <string> cardTreasure;
    static const vector <string> cardFlood;
    static const vector <string> cardIsland;
    static const vector <string> cardIslandFlood;
    static const vector <string> cardPlayer;
    static const vector <string> treasure;
    QPixmap cardImageTreasure[8];
    QPixmap cardImageFlood[25];
    QPixmap cardImageIsland[24];
    QPixmap cardImageIslandFlood[24];
    QPixmap cardImagePlayer[6];
    QPixmap treasureImage[4];
    QPixmap water;
    QPushButton* m_iC[36];
    Game* m_pGame;
    QPushButton* m_playerCards[4][6];
    QPushButton* m_player[4];
    QPushButton* m_floodDecks[2];
    QPushButton* m_treasureDecks[2];
    QPushButton* m_pawns[4][36];
    QPushButton* m_treasure[4];
    QLabel* logo;
    QLabel* dialog;
    QPushButton* m_dialog[7];
    int dialogMode = 0;
    int squaresToFlood = 6;
    vector <int> validSquares = {2,3,7,8,9,10,12,13,14,15,16,17,18,19,20,21,22,23,25,26,27,28,32,33};
    vector <int> invalidSquares = {0,1,4,5,6,11,24,29,30,31,34,35};
    int playerAction = 9;
    int activePlayerSlot;
    vector <int> validMoves;
    QLabel* m_wrCard;
    QLabel* m_wrIndicator;
    bool engineerSU = false;
    bool fly = false;
    bool sendTreasure = false;
    bool giveTreasure = false;
    vector <int> sameTile;
    bool moveOther = false;
    bool playerPicked = false;
    int  receivingPlayer;
    int  mSlot;
    bool firstMove = false;
    bool getTreasure = false;
    int  treasureValue;
    bool helo = false;
    int heloPlayer;
    int heloSlot;
    int  heloFrom = 0;
    bool heloPlayersSelected = false;
    vector <int> heloGroup;
    vector <int> heloPlayers;
    bool sandBag = false;
    int sandBagPlayer;
    int sandBagSlot;
    int cardsDrawn = 0;
    bool waterRise = false;
};
#endif // FORBIDDENISLAND_H
