#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QEventLoop>
#include <string.h>
#include <QKeyEvent>

#include "../Package.h"
#include "../read_kb.h"
#include "../rs232.h"

int flag = 0;
int fd_RS232;
QByteArray pressed_key;
BYTE QRMode = MODE_SAFE; //Initializes QR Mode

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QEventLoop(&ui);
    // INITIALIZES CONTROLS
    ui->Lift->setText("0");
    ui->Roll->setText("0");
    ui->Pitch->setText("0");
    ui->Yaw->setText("0");
    ui->P->setText("0");
    ui->P1->setText("0");
    ui->P2->setText("0");

    //focus on one of the controls
    ui->Lift->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_RunButt_clicked()
{
    //RUN OR STOP DATA TRANSFER
    int i = 0;
    if (ui->RunButt->text() == "Run"){
        //Start sending data
        ui->StatusLbl->setText("Running...");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : orange}");
        ui->RunButt->setText("Stop");
        flag = 0;
    }
    else
    {
        //Stops sending data
        ui->StatusLbl->setText("Stopped");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : white}");
        ui->RunButt->setText("Run");
        flag = 1;
    }

    //PACKAGE
    int keymap[8] = {MODE_SAFE,0,0,0,0,0,0,0}; //initializes keyboard input
    int jmap[4] = {0,0,0,0}; //initializes joystick input
    int data[PARAM_LENGTH] = {0,0,0,0}; //initializes Package data
    Package mPkg;
    InitPkg(&mPkg,QRMode); //Initializes Package

    int c;
    while(flag == 0)
    {
        QCoreApplication::processEvents();
//        keyboard_nonblocking(); //Prevent for keyboard blocking
//        read_js(jmap);

        if (pressed_key != -1) read_kb(keymap,pressed_key);
        QRMode = keymap[0];
        switch (QRMode) {
            case MODE_P:
                data[1] = keymap[5];
                data[2] = keymap[6];
                data[3] = keymap[7];
                data[0] = 0;
                ui->P->setText(QString::number(data[0]));
                break;

            default: //CONTROL MODES
                //Update Package Data
                data[0] = jmap[0]+keymap[1];
                data[1] = jmap[1]+keymap[2];
                data[2] = jmap[2]+keymap[3];
                data[3] = jmap[3]+keymap[4];
                //Update Sent Info in GUI
                ui->Lift->setText(QString::number(data[0]));
                ui->Roll->setText(QString::number(data[1]));
                ui->Pitch->setText(QString::number(data[2]));
                ui->Yaw->setText(QString::number(data[3]));
                break;
        }

        //CREATES THE PACKAGE
        SetPkgMode(&mPkg,QRMode);
        SetPkgData(&mPkg,data);

        //SEND PACKAGE HERE
        //writes in the port
//        write(fd_RS232,mPkg.Pkg,6*sizeof(BYTE));

        ui->PkgTxtBox->setText(QString::number(mPkg.ChkSum));

        //        usleep(10000);

        i++;
    }

//    MsgBox("Test");
}



void MainWindow::on_pushButton_2_clicked()
{
    if (flag == 0) {
        flag = 1;
    }
    else
    {
        flag = 0;

    }
}

void MainWindow::on_ModeSel_currentIndexChanged(int index)
{
//    ui->StatusLbl->setText(ui->ModeSel->currentText());
    switch (index)//ui->ModeSel->currentText())
    {
    case 0:
        QRMode = MODE_SAFE;
        break;
    case 1:
        QRMode = MODE_PANIC;
        break;
    case 2:
        QRMode = MODE_MANUAL;
        break;
    case 3:
        QRMode = MODE_CALIBRATION;
        break;
    case 4:
        QRMode = MODE_YAW_CONTROL;
        break;
    case 5:
        QRMode = MODE_FULL_CONTROL;
        break;
    case 6:
        QRMode = MODE_P;
        break;
    default:
        QRMode = MODE_SAFE;
        break;
    }
}

QString getStringFromUnsignedChar(unsigned char *str)
{

    QString s;
    QString result = "";
//    int rev = strlen(str);

    // Print String in Reverse order....
    for ( int i = 0; i<7; i++)
        {
           s = QString("%1").arg(str[i],0,16);

           if(s == "0"){
              s="00";
             }
         result.append(s);

         }
   return result;
}

void MainWindow::on_CommButt_clicked()
{
    if (ui->CommButt->text() == "Connect"){
        //OPEN HERE COM PORT
        ui->StatusLbl->setText("Opening Port...");
        fd_RS232 = open_rs232_port();
        if (fd_RS232 == 0)
        {
            ui->StatusLbl->setStyleSheet("QLabel { background-color : red}");
            ui->StatusLbl->setText("Error Opening  the Port!!!");
            return;
        }
        ui->RunButt->setEnabled(1);
        ui->StatusLbl->setText("Connected!");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : green}");
        ui->CommButt->setText("Disconnect");
        flag = 0;
    }
    else
    {
        //CLOSE HERE PORT
        ui->RunButt->setEnabled(0);
        ui->StatusLbl->setText("Closing the port");
        close_rs232_port(fd_RS232);
        ui->StatusLbl->setText("Port Closed!");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : white}");
        ui->CommButt->setText("Connect");
        flag = 1;
    }
}


void MainWindow::keyPressEvent(QKeyEvent* Key)
{
//    ui->StatusLbl->setText(Key->text().toLower());
    pressed_key = Key->text().toAscii();//.toInt();
    ui->StatusLbl->setText(QString::number(pressed_key));
}
