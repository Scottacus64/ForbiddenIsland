/********************************************************************************
** Form generated from reading UI file 'solitaire.ui'
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

QT_BEGIN_NAMESPACE

class Ui_SolitaireUI
{
public:
    QLabel *c0;
    QLabel *c1;
    QLabel *a0;

    void setupUi(QWidget *SolitaireUI)
    {
        if (SolitaireUI->objectName().isEmpty())
            SolitaireUI->setObjectName("SolitaireUI");
        SolitaireUI->resize(1087, 700);
        c0 = new QLabel(SolitaireUI);
        c0->setObjectName("c0");
        c0->setGeometry(QRect(30, 160, 100, 140));
        c0->setPixmap(QPixmap(QString::fromUtf8("../../Solitaire/CardPNGs/1S.png")));
        c0->setScaledContents(true);
        c1 = new QLabel(SolitaireUI);
        c1->setObjectName("c1");
        c1->setGeometry(QRect(30, 190, 100, 140));
        c1->setPixmap(QPixmap(QString::fromUtf8("../../Solitaire/CardPNGs/1S.png")));
        c1->setScaledContents(true);
        a0 = new QLabel(SolitaireUI);
        a0->setObjectName("a0");
        a0->setGeometry(QRect(260, 0, 100, 140));
        a0->setPixmap(QPixmap(QString::fromUtf8("../../Solitaire/CardPNGs/1S.png")));
        a0->setScaledContents(true);

        retranslateUi(SolitaireUI);

        QMetaObject::connectSlotsByName(SolitaireUI);
    } // setupUi

    void retranslateUi(QWidget *SolitaireUI)
    {
        SolitaireUI->setWindowTitle(QCoreApplication::translate("SolitaireUI", "SolitaireUI", nullptr));
        c0->setText(QString());
        c1->setText(QString());
        a0->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SolitaireUI: public Ui_SolitaireUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOLITAIRE_H
