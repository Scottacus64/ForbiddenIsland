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
    /*void refreshUpperSection();
    void dealCards();
    void updateDecks(int deck, int dCards);
    void postWin();
    void enableDrawPile(int pile, int id);
    void disableDrawPile(int pile);

    void autoFinish();
    void delayTimer(int delay);
    void undoPressed();
    void newGamePressed();
    void refreshDecks();*/
    
public slots:
    void dialogButtonClicked();
    void pawnClicked();
    void iTileClicked();
    /*void cardClicked();
    void easyClicked();
    void hardClicked();*/

private:
    Ui::ForbiddenIslandUI *ui;
    static const vector <string> cardTreasure;
    static const vector <string> cardFlood;
    static const vector <string> cardIsland;
    static const vector <string> cardIslandFlood;
    static const vector <string> cardPlayer;
    QPixmap cardImageTreasure[8];
    QPixmap cardImageFlood[25];
    QPixmap cardImageIsland[24];
    QPixmap cardImageIslandFlood[24];
    QPixmap cardImagePlayer[6];
    QPixmap water;
    QPushButton* m_iC[36];
    Game* m_pGame;
    QPushButton* m_playerCards[4][6];
    QPushButton* m_player[4];
    QPushButton* m_floodDecks[2];
    QPushButton* m_treasureDecks[2];
    QPushButton* m_pawns[4][36];
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
    /*QPushButton* m_pA[4];
    QPushButton* m_pC[133];
    QPushButton* m_pD[4];
    QPushButton* m_undo;
    QPushButton* m_newGame;
    QPushButton* m_easy;
    QPushButton* m_hard;
    QTimer elapsedTime;
    QLabel* m_startLogo;
    QLabel* m_moves;
    QLabel* m_winMoves;
    QLabel* m_timer;
    QLabel* m_winTime;
    QLabel* m_noMovesLeft;
    QLabel* m_winScreen[5];
    QTimer timer;
    QElapsedTimer elapsedTimer;
    //Solitaire* m_pSolitaire;
    int cardsDelt = 0;
    bool gameStarted = false;
    qint64 elapsedMilliseconds;
    int seconds;
    std::vector<int> timeVec;
    std::vector<int> moveVec;
    bool win = false;*/
};
#endif // FORBIDDENISLAND_H
