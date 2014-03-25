/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Mar 25 19:42:12 2014
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
    QLCDNumber *Lift;
    QLCDNumber *Roll;
    QLCDNumber *Pitch;
    QLCDNumber *Yaw;
    QLCDNumber *P;
    QLCDNumber *P1;
    QLCDNumber *P2;
    QGroupBox *groupBox_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QWidget *layoutWidget1;
    QVBoxLayout *TelLay;
    QLCDNumber *Tel1;
    QLCDNumber *Tel2;
    QLCDNumber *Tel4;
    QLCDNumber *Tel3;
    QLCDNumber *Tel5;
    QLCDNumber *Tel6;
    QSplitter *splitter_2;
    QPushButton *CommButt;
    QPushButton *RunButt;
    QPushButton *SaveDL;
    QPushButton *Abort;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(831, 544);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        StatusLbl = new QLabel(centralWidget);
        StatusLbl->setObjectName(QString::fromUtf8("StatusLbl"));
        StatusLbl->setGeometry(QRect(10, 460, 811, 21));
        StatusLbl->setAcceptDrops(false);
        StatusLbl->setFrameShape(QFrame::StyledPanel);
        StatusLbl->setFrameShadow(QFrame::Plain);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 0, 221, 411));
        gridLayoutWidget = new QWidget(groupBox);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 30, 201, 331));
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

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(260, 0, 261, 361));
        groupBox_2->setAlignment(Qt::AlignJustify|Qt::AlignTop);
        groupBox_2->setFlat(false);
        groupBox_2->setCheckable(false);
        splitter = new QSplitter(groupBox_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(20, 30, 211, 251));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        verticalLayout_3->addWidget(label_9);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        verticalLayout_3->addWidget(label_10);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        verticalLayout_3->addWidget(label_11);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        verticalLayout_3->addWidget(label_12);

        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout_3->addWidget(label_13);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        verticalLayout_3->addWidget(label_14);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        TelLay = new QVBoxLayout(layoutWidget1);
        TelLay->setSpacing(6);
        TelLay->setContentsMargins(11, 11, 11, 11);
        TelLay->setObjectName(QString::fromUtf8("TelLay"));
        TelLay->setContentsMargins(0, 0, 0, 0);
        Tel1 = new QLCDNumber(layoutWidget1);
        Tel1->setObjectName(QString::fromUtf8("Tel1"));
        Tel1->setFrameShape(QFrame::StyledPanel);
        Tel1->setFrameShadow(QFrame::Raised);
        Tel1->setMode(QLCDNumber::Hex);

        TelLay->addWidget(Tel1);

        Tel2 = new QLCDNumber(layoutWidget1);
        Tel2->setObjectName(QString::fromUtf8("Tel2"));
        Tel2->setFrameShape(QFrame::StyledPanel);
        Tel2->setFrameShadow(QFrame::Raised);
        Tel2->setMode(QLCDNumber::Hex);

        TelLay->addWidget(Tel2);

        Tel4 = new QLCDNumber(layoutWidget1);
        Tel4->setObjectName(QString::fromUtf8("Tel4"));
        Tel4->setFrameShape(QFrame::StyledPanel);
        Tel4->setFrameShadow(QFrame::Raised);
        Tel4->setMode(QLCDNumber::Hex);

        TelLay->addWidget(Tel4);

        Tel3 = new QLCDNumber(layoutWidget1);
        Tel3->setObjectName(QString::fromUtf8("Tel3"));
        Tel3->setFrameShape(QFrame::StyledPanel);
        Tel3->setFrameShadow(QFrame::Raised);
        Tel3->setMode(QLCDNumber::Hex);

        TelLay->addWidget(Tel3);

        Tel5 = new QLCDNumber(layoutWidget1);
        Tel5->setObjectName(QString::fromUtf8("Tel5"));
        Tel5->setFrameShape(QFrame::StyledPanel);
        Tel5->setFrameShadow(QFrame::Raised);
        Tel5->setMode(QLCDNumber::Hex);

        TelLay->addWidget(Tel5);

        Tel6 = new QLCDNumber(layoutWidget1);
        Tel6->setObjectName(QString::fromUtf8("Tel6"));
        Tel6->setFrameShape(QFrame::StyledPanel);
        Tel6->setFrameShadow(QFrame::Raised);
        Tel6->setMode(QLCDNumber::Hex);

        TelLay->addWidget(Tel6);

        splitter->addWidget(layoutWidget1);
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(580, 30, 221, 401));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setOrientation(Qt::Vertical);
        splitter_2->setHandleWidth(25);
        CommButt = new QPushButton(splitter_2);
        CommButt->setObjectName(QString::fromUtf8("CommButt"));
        splitter_2->addWidget(CommButt);
        RunButt = new QPushButton(splitter_2);
        RunButt->setObjectName(QString::fromUtf8("RunButt"));
        RunButt->setEnabled(false);
        splitter_2->addWidget(RunButt);
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
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 831, 25));
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
        StatusLbl->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Sent Parameters", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Mode", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Lift", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Roll", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Pitch", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Yaw", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "P", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "P1", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "P2", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Telemetry", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "S1", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "S2", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "S3", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "S4", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "S5", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "S6", 0, QApplication::UnicodeUTF8));
        CommButt->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        RunButt->setToolTip(QApplication::translate("MainWindow", "Controls the QR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        RunButt->setText(QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
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
