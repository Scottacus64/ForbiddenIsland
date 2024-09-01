#ifndef SOLITAIREUI_H
#define SOLITAIREUI_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>
//#include "ui_solitaire.h"
#include "Solitaire.h"
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class SolitaireUI; }
QT_END_NAMESPACE

class SolitaireUI : public QWidget
{
    Q_OBJECT


public:
    SolitaireUI(QWidget *parent = nullptr);
    ~SolitaireUI();
    void refreshUpperSection();
    void dealCards();
    void updateDecks(int deck, int dCards);
    void postWin();
    void enableDrawPile(int pile, int id);
    void disableDrawPile(int pile);
    void paintEvent(QPaintEvent *event);
    void autoFinish();
    void delayTimer(int delay);
    void undoPressed();
    void newGamePressed();
    void refreshDecks();
    
public slots:
    void cardClicked();
    void easyClicked();
    void hardClicked();

private:
    Ui::SolitaireUI *ui;
    QPixmap cardImage[53];
    QPixmap green;
    QPushButton* m_pA[4];
    QPushButton* m_pC[133];
    QPushButton* m_pD[4];
    QPushButton* m_undo;
    QPushButton* m_newGame;
    QPushButton* m_easy;
    QPushButton* m_hard;
    QTimer elapsedTime;
    QLabel* m_startLogo;
    QLabel* m_moves;
    QLabel* m_timer;
    QLabel* m_noMovesLeft;
    QLabel* m_winScreen[5];
    QTimer timer;
    QElapsedTimer elapsedTimer;
    Solitaire* m_pSolitaire;
    int cardsDelt = 0;
    bool gameStarted = false;
    qint64 elapsedMilliseconds;
    int seconds;
    std::vector<int> timeVec;
    std::vector<int> moveVec;
    bool win = false;
};
#endif // SOLITAIREUI_H
