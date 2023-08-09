/********************************************************************************
** Form generated from reading UI file 'solitaireui.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOLITAIRE_H
#define UI_SOLITAIRE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SolitaireUI
{

public:
    QLabel *c0;
    QLabel *c1;
    QLabel *a0;
    QPushButton* m_pC[133];
    QPushButton* m_pA[4];
    QPushButton* m_pD[2];

    void setupUi(QWidget *SolitaireUI)
    {
        if (SolitaireUI->objectName().isEmpty())
        {
            SolitaireUI->setObjectName("SolitaireUI");
            SolitaireUI->resize(1087, 1000);
            SolitaireUI->move(700, 0);
        }
    } 
};

namespace Ui {
    class SolitaireUI: public Ui_SolitaireUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLITAIRE_H
