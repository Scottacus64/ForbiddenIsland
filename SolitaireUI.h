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
    void updateDecks(int deck, int cards);
    void checkForWin();

public slots:
    void cardClicked();

private:
    Ui::SolitaireUI *ui;
    QPixmap cardImage[53];

    QPushButton* m_pA[4];
    QPushButton* m_pC[133];
    QPushButton* m_pD[2];
    QPushButton* m_newGame;

    Solitaire* m_pSolitaire;
};
#endif // SOLITAIREUI_H
