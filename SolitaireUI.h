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
    void refreshDecks();


public slots:
    void cardClicked();
    void winnableClicked();
    void randomClicked();

private:
    Ui::SolitaireUI *ui;
    QPixmap cardImage[53];
    QPixmap green;
    QPushButton* m_pA[4];
    QPushButton* m_pC[133];
    QPushButton* m_pD[4];
    QPushButton* m_undo;
    QPushButton* m_winnable;
    QPushButton* m_random;
    QTimer elapsedTime;
    QLabel* m_moves;
    QLabel* m_timer;
    QLabel* m_noMovesLeft;
    QTimer timer;
    QElapsedTimer elapsedTimer;
    Solitaire* m_pSolitaire;
    int cardsDelt = 0;
    bool gameStarted;
    qint64 elapsedMilliseconds;
};
#endif // SOLITAIREUI_H
