#ifndef FORBIDDENISLANDUI_H
#define FORBIDDENISLANDUI_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

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
    /*void cardClicked();
    void easyClicked();
    void hardClicked();*/

private:
    Ui::ForbiddenIslandUI *ui;
    static const vector <string> cardTreasure;
    static const vector <string> cardFlood;
    static const vector <string> cardIsland;
    static const vector <string> cardPlayer;
    const vector <int> invalidSquares;
    QPixmap cardImageTreasure[8];
    QPixmap cardImageFlood[25];
    QPixmap cardImageIsland[48];
    QPixmap cardImagePlayer[6];
    QPixmap water;
    QPushButton* m_iC[24];
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
