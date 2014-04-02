/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Apr 2 10:29:48 2014
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
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
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
    QLCDNumber *Lift;
    QLCDNumber *Roll;
    QLCDNumber *Pitch;
    QLCDNumber *Yaw;
    QLCDNumber *P;
    QLCDNumber *P1;
    QLCDNumber *P2;
    QLabel *QRImage;
    QLCDNumber *Pitch_2;
    QLCDNumber *Yaw_2;
    QLCDNumber *Lift_2;
    QLCDNumber *Roll_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_24;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_23;
    QLCDNumber *Lift_3;
    QLCDNumber *Roll_3;
    QLCDNumber *Pitch_3;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_25;
    QLCDNumber *Lift_4;
    QLCDNumber *Roll_4;
    QLCDNumber *Pitch_4;
    QSplitter *splitter_3;
    QSplitter *splitter;
    QPushButton *CommButt;
    QPushButton *RunButt;
    QSplitter *splitter_2;
    QPushButton *SaveDL;
    QPushButton *Abort;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(845, 711);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 0, 221, 351));
        gridLayoutWidget = new QWidget(groupBox);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 20, 201, 331));
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
        ModeSel->setEnabled(false);
        ModeSel->setFocusPolicy(Qt::NoFocus);
        ModeSel->setFrame(true);
        ModeSel->setReadOnly(true);

        verticalLayout->addWidget(ModeSel);

        Lift = new QLCDNumber(gridLayoutWidget);
        Lift->setObjectName(QString::fromUtf8("Lift"));
        Lift->setFrameShape(QFrame::StyledPanel);
        Lift->setFrameShadow(QFrame::Raised);
        Lift->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(Lift);

        Roll = new QLCDNumber(gridLayoutWidget);
        Roll->setObjectName(QString::fromUtf8("Roll"));
        Roll->setFrameShape(QFrame::StyledPanel);
        Roll->setFrameShadow(QFrame::Raised);
        Roll->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(Roll);

        Pitch = new QLCDNumber(gridLayoutWidget);
        Pitch->setObjectName(QString::fromUtf8("Pitch"));
        Pitch->setFrameShape(QFrame::StyledPanel);
        Pitch->setFrameShadow(QFrame::Raised);
        Pitch->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(Pitch);

        Yaw = new QLCDNumber(gridLayoutWidget);
        Yaw->setObjectName(QString::fromUtf8("Yaw"));
        Yaw->setFrameShape(QFrame::StyledPanel);
        Yaw->setFrameShadow(QFrame::Raised);
        Yaw->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(Yaw);

        P = new QLCDNumber(gridLayoutWidget);
        P->setObjectName(QString::fromUtf8("P"));
        P->setFrameShape(QFrame::StyledPanel);
        P->setFrameShadow(QFrame::Raised);
        P->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(P);

        P1 = new QLCDNumber(gridLayoutWidget);
        P1->setObjectName(QString::fromUtf8("P1"));
        P1->setFrameShape(QFrame::StyledPanel);
        P1->setFrameShadow(QFrame::Raised);
        P1->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(P1);

        P2 = new QLCDNumber(gridLayoutWidget);
        P2->setObjectName(QString::fromUtf8("P2"));
        P2->setFrameShape(QFrame::StyledPanel);
        P2->setFrameShadow(QFrame::Raised);
        P2->setMode(QLCDNumber::Dec);

        verticalLayout->addWidget(P2);


        gridLayout->addLayout(verticalLayout, 0, 1, 1, 1);

        QRImage = new QLabel(centralWidget);
        QRImage->setObjectName(QString::fromUtf8("QRImage"));
        QRImage->setGeometry(QRect(460, 360, 251, 231));
        QRImage->setFrameShape(QFrame::Box);
        QRImage->setPixmap(QPixmap(QString::fromUtf8("QRChamp.png")));
        QRImage->setScaledContents(true);
        QRImage->setAlignment(Qt::AlignCenter);
        Pitch_2 = new QLCDNumber(centralWidget);
        Pitch_2->setObjectName(QString::fromUtf8("Pitch_2"));
        Pitch_2->setGeometry(QRect(520, 600, 111, 37));
        Pitch_2->setFrameShape(QFrame::StyledPanel);
        Pitch_2->setFrameShadow(QFrame::Raised);
        Pitch_2->setMode(QLCDNumber::Dec);
        Yaw_2 = new QLCDNumber(centralWidget);
        Yaw_2->setObjectName(QString::fromUtf8("Yaw_2"));
        Yaw_2->setGeometry(QRect(710, 450, 111, 36));
        Yaw_2->setFrameShape(QFrame::StyledPanel);
        Yaw_2->setFrameShadow(QFrame::Raised);
        Yaw_2->setMode(QLCDNumber::Dec);
        Lift_2 = new QLCDNumber(centralWidget);
        Lift_2->setObjectName(QString::fromUtf8("Lift_2"));
        Lift_2->setGeometry(QRect(520, 320, 111, 37));
        Lift_2->setFrameShape(QFrame::StyledPanel);
        Lift_2->setFrameShadow(QFrame::Raised);
        Lift_2->setMode(QLCDNumber::Dec);
        Roll_2 = new QLCDNumber(centralWidget);
        Roll_2->setObjectName(QString::fromUtf8("Roll_2"));
        Roll_2->setGeometry(QRect(350, 450, 101, 41));
        Roll_2->setFrameShape(QFrame::StyledPanel);
        Roll_2->setFrameShadow(QFrame::Raised);
        Roll_2->setMode(QLCDNumber::Dec);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 380, 301, 171));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(6);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_24 = new QLabel(layoutWidget);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        verticalLayout_8->addWidget(label_24);

        label_19 = new QLabel(layoutWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        verticalLayout_8->addWidget(label_19);

        label_20 = new QLabel(layoutWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        verticalLayout_8->addWidget(label_20);

        label_21 = new QLabel(layoutWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        verticalLayout_8->addWidget(label_21);


        gridLayout_2->addLayout(verticalLayout_8, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_23 = new QLabel(layoutWidget);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        verticalLayout_4->addWidget(label_23);

        Lift_3 = new QLCDNumber(layoutWidget);
        Lift_3->setObjectName(QString::fromUtf8("Lift_3"));
        Lift_3->setFrameShape(QFrame::StyledPanel);
        Lift_3->setFrameShadow(QFrame::Raised);
        Lift_3->setMode(QLCDNumber::Dec);

        verticalLayout_4->addWidget(Lift_3);

        Roll_3 = new QLCDNumber(layoutWidget);
        Roll_3->setObjectName(QString::fromUtf8("Roll_3"));
        Roll_3->setFrameShape(QFrame::StyledPanel);
        Roll_3->setFrameShadow(QFrame::Raised);
        Roll_3->setMode(QLCDNumber::Dec);

        verticalLayout_4->addWidget(Roll_3);

        Pitch_3 = new QLCDNumber(layoutWidget);
        Pitch_3->setObjectName(QString::fromUtf8("Pitch_3"));
        Pitch_3->setFrameShape(QFrame::StyledPanel);
        Pitch_3->setFrameShadow(QFrame::Raised);
        Pitch_3->setMode(QLCDNumber::Dec);

        verticalLayout_4->addWidget(Pitch_3);


        gridLayout_2->addLayout(verticalLayout_4, 0, 1, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_25 = new QLabel(layoutWidget);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setLayoutDirection(Qt::LeftToRight);
        label_25->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_25);

        Lift_4 = new QLCDNumber(layoutWidget);
        Lift_4->setObjectName(QString::fromUtf8("Lift_4"));
        Lift_4->setFrameShape(QFrame::StyledPanel);
        Lift_4->setFrameShadow(QFrame::Raised);
        Lift_4->setMode(QLCDNumber::Dec);

        verticalLayout_5->addWidget(Lift_4);

        Roll_4 = new QLCDNumber(layoutWidget);
        Roll_4->setObjectName(QString::fromUtf8("Roll_4"));
        Roll_4->setFrameShape(QFrame::StyledPanel);
        Roll_4->setFrameShadow(QFrame::Raised);
        Roll_4->setMode(QLCDNumber::Dec);

        verticalLayout_5->addWidget(Roll_4);

        Pitch_4 = new QLCDNumber(layoutWidget);
        Pitch_4->setObjectName(QString::fromUtf8("Pitch_4"));
        Pitch_4->setFrameShape(QFrame::StyledPanel);
        Pitch_4->setFrameShadow(QFrame::Raised);
        Pitch_4->setMode(QLCDNumber::Dec);

        verticalLayout_5->addWidget(Pitch_4);


        gridLayout_2->addLayout(verticalLayout_5, 0, 2, 1, 1);

        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setGeometry(QRect(320, 30, 431, 171));
        splitter_3->setFrameShape(QFrame::NoFrame);
        splitter_3->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        CommButt = new QPushButton(splitter);
        CommButt->setObjectName(QString::fromUtf8("CommButt"));
        splitter->addWidget(CommButt);
        RunButt = new QPushButton(splitter);
        RunButt->setObjectName(QString::fromUtf8("RunButt"));
        RunButt->setEnabled(false);
        splitter->addWidget(RunButt);
        splitter_3->addWidget(splitter);
        splitter_2 = new QSplitter(splitter_3);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        SaveDL = new QPushButton(splitter_2);
        SaveDL->setObjectName(QString::fromUtf8("SaveDL"));
        SaveDL->setEnabled(false);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 127, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(255, 63, 63, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(127, 0, 0, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(170, 0, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        SaveDL->setPalette(palette);
        splitter_2->addWidget(SaveDL);
        Abort = new QPushButton(splitter_2);
        Abort->setObjectName(QString::fromUtf8("Abort"));
        Abort->setEnabled(false);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        Abort->setPalette(palette1);
        splitter_2->addWidget(Abort);
        splitter_3->addWidget(splitter_2);
        MainWindow->setCentralWidget(centralWidget);
        Abort->raise();
        RunButt->raise();
        groupBox->raise();
        QRImage->raise();
        Pitch_2->raise();
        Yaw_2->raise();
        Lift_2->raise();
        Roll_2->raise();
        layoutWidget->raise();
        SaveDL->raise();
        CommButt->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 845, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(RunButt, ModeSel);
        QWidget::setTabOrder(ModeSel, CommButt);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Sent Parameters", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Lift", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Roll", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Pitch", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Yaw", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "P", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "P1", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "P2", 0, QApplication::UnicodeUTF8));
        QRImage->setText(QString());
        label_24->setText(QString());
        label_19->setText(QApplication::translate("MainWindow", "r", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "phi", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MainWindow", "theta", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "REFERENCES", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("MainWindow", "QR", 0, QApplication::UnicodeUTF8));
        CommButt->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        RunButt->setToolTip(QApplication::translate("MainWindow", "Controls the QR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        RunButt->setText(QApplication::translate("MainWindow", "Start Sending...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        SaveDL->setToolTip(QApplication::translate("MainWindow", "Controls the QR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        SaveDL->setText(QApplication::translate("MainWindow", "Download DataLog", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        Abort->setToolTip(QApplication::translate("MainWindow", "Controls the QR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Abort->setText(QApplication::translate("MainWindow", "Abort", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
