#ifndef SOLITAIREUI_H
#define SOLITAIREUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "ui_solitaire.h"
#include "Solitaire.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SolitaireUI; }
QT_END_NAMESPACE

class SolitaireUI : public QWidget
{
    Q_OBJECT


public:
    SolitaireUI(QWidget *parent = nullptr);
    ~SolitaireUI();
    void refreshScreen();
    void dealCards();
    void updateDecks(int deck, int cardsDelt);
    void checkForWin();
    void enableDrawPile(int pile, int id);
    void disableDrawPile(int pile);
    void paintEvent(QPaintEvent *event);
    void autoFinish();

public slots:
    void cardClicked();

private:
    Ui::SolitaireUI *ui;
    QPixmap cardImage[53];
    QPixmap green;

    QPushButton* m_pA[4];
    QPushButton* m_pC[133];
    QPushButton* m_pD[4];
    QPushButton* m_newGame;

    Solitaire* m_pSolitaire;
    int cardsDelt = 0;
    bool drawPileFlag;
};
#endif // SOLITAIREUI_H
