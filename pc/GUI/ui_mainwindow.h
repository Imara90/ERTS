/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Mar 17 20:17:45 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *RunButt;
    QLabel *StatusLbl;
    QGroupBox *groupBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_6;
    QLabel *label_7;
    QVBoxLayout *verticalLayout;
    QLineEdit *ModeSel;
    QLineEdit *Lift;
    QLineEdit *Roll;
    QLineEdit *Pitch;
    QLineEdit *Yaw;
    QLineEdit *P;
    QLineEdit *P1;
    QLineEdit *P2;
    QPushButton *pushButton_2;
    QLineEdit *PkgTxtBox;
    QPushButton *CommButt;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(655, 544);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        RunButt = new QPushButton(centralWidget);
        RunButt->setObjectName(QString::fromUtf8("RunButt"));
        RunButt->setEnabled(false);
        RunButt->setGeometry(QRect(320, 160, 211, 41));
        StatusLbl = new QLabel(centralWidget);
        StatusLbl->setObjectName(QString::fromUtf8("StatusLbl"));
        StatusLbl->setGeometry(QRect(10, 460, 631, 21));
        StatusLbl->setAcceptDrops(false);
        StatusLbl->setFrameShape(QFrame::StyledPanel);
        StatusLbl->setFrameShadow(QFrame::Plain);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 251, 281));
        gridLayoutWidget = new QWidget(groupBox);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 30, 201, 262));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_2->addWidget(label_5);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        verticalLayout_2->addWidget(label_8);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_2->addWidget(label_6);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_2->addWidget(label_7);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ModeSel = new QLineEdit(gridLayoutWidget);
        ModeSel->setObjectName(QString::fromUtf8("ModeSel"));
        ModeSel->setReadOnly(true);

        verticalLayout->addWidget(ModeSel);

        Lift = new QLineEdit(gridLayoutWidget);
        Lift->setObjectName(QString::fromUtf8("Lift"));
        Lift->setReadOnly(true);

        verticalLayout->addWidget(Lift);

        Roll = new QLineEdit(gridLayoutWidget);
        Roll->setObjectName(QString::fromUtf8("Roll"));
        Roll->setReadOnly(true);

        verticalLayout->addWidget(Roll);

        Pitch = new QLineEdit(gridLayoutWidget);
        Pitch->setObjectName(QString::fromUtf8("Pitch"));
        Pitch->setReadOnly(true);

        verticalLayout->addWidget(Pitch);

        Yaw = new QLineEdit(gridLayoutWidget);
        Yaw->setObjectName(QString::fromUtf8("Yaw"));
        Yaw->setReadOnly(true);

        verticalLayout->addWidget(Yaw);

        P = new QLineEdit(gridLayoutWidget);
        P->setObjectName(QString::fromUtf8("P"));
        P->setReadOnly(true);

        verticalLayout->addWidget(P);

        P1 = new QLineEdit(gridLayoutWidget);
        P1->setObjectName(QString::fromUtf8("P1"));
        P1->setReadOnly(true);

        verticalLayout->addWidget(P1);

        P2 = new QLineEdit(gridLayoutWidget);
        P2->setObjectName(QString::fromUtf8("P2"));
        P2->setReadOnly(true);

        verticalLayout->addWidget(P2);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(320, 210, 211, 41));
        PkgTxtBox = new QLineEdit(centralWidget);
        PkgTxtBox->setObjectName(QString::fromUtf8("PkgTxtBox"));
        PkgTxtBox->setGeometry(QRect(30, 310, 231, 21));
        CommButt = new QPushButton(centralWidget);
        CommButt->setObjectName(QString::fromUtf8("CommButt"));
        CommButt->setGeometry(QRect(320, 110, 211, 41));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 655, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        RunButt->setToolTip(QApplication::translate("MainWindow", "Controls the QR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        RunButt->setText(QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
        StatusLbl->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "GroupBox", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Lift", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Roll", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Pitch", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Yaw", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "P", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "P1", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "P2", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MainWindow", "Calibration", 0, QApplication::UnicodeUTF8));
        CommButt->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
