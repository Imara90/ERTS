#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QEventLoop>
#include <string.h>
#include <QKeyEvent>

#include "../read_kb.h"
#include "../rs232.h"
#include "../Package.h"


int flag = 0;
int fd_RS232;
char pressed_key = -1;
unsigned char  QRMode = MODE_SAFE; //Initializes QR Mode

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
    int result;
    if (ui->RunButt->text() == "Run"){
        //Start sending data
        ui->StatusLbl->setText("Running...");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : orange}");
        ui->RunButt->setText("Stop");
//        fd_rs232 = open_rs232_port();
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
//        read_js(jmap);

        if (pressed_key != -1) {
            read_kb(keymap,&pressed_key);
            printf("\n Key %i",pressed_key);
        }

        QRMode = keymap[0];
//        ui->ModeSel->setText(GetMode(QRMode));
        switch (QRMode) {
            case MODE_P:
                data[1] = keymap[5];
                data[2] = keymap[6];
                data[3] = keymap[7];
                data[0] = 0;
                ui->P->setText(QString::number(data[1]));
                break;
            default: //CONTROL MODES
                //Update Package Data
                data[0] = jmap[0]+keymap[1];
                data[1] = jmap[1]+keymap[2];
                data[2] = jmap[2]+keymap[3];
                data[3] = jmap[3]+keymap[4];
                //Update MODE_PSent Info in GUI
                ui->Lift->setText(QString::number(data[0]));
                ui->Roll->setText(QString::number(data[1]));
                ui->Pitch->setText(QString::number(data[2]));
                ui->Yaw->setText(QString::number(data[3]));
                break;
        }

        //CREATES THE PACKAGE
        SetPkgMode(&mPkg,QRMode);
        SetPkgData(&mPkg,data);
        //Prints the package
        for (i = 0; i < PKGLEN; i++) {
            printf("[%x]",mPkg.Pkg[i]);
        }
        printf("\n");

        //SEND PACKAGE HERE
        //writes in the port
        result = write(fd_RS232,mPkg.Pkg,7*sizeof(unsigned char ));
        //Asserts in case of sending wrong number of bytes
        assert(result == 7);

        ui->PkgTxtBox->setText(QString::number(mPkg.ChkSum));

        usleep(10000);

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


//QString getStringFromUnsignedChar(unsigned char *str)
//{

//    QString s;
//    QString result = "";
////    int rev = strlen(str);

//    // Print String in Reverse order....
//    for ( int i = 0; i<7; i++)
//        {
//           s = QString("%1").arg(str[i],0,16);

//           if(s == "0"){
//              s="00";
//             }
//         result.append(s);

//         }
//   return result;
//}

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
    ui->StatusLbl->setText(QString::number(Key->key()));
    switch (Key->key()){
        case Qt::Key_0:
            pressed_key = '0';
            break;
        case Qt::Key_1:
            pressed_key = '1';
            break;
        case Qt::Key_2:
            pressed_key = '2';
            break;
        case Qt::Key_3:
            pressed_key = '3';
            break;
        case Qt::Key_4:
            pressed_key = '4';
            break;
        case Qt::Key_5:
            pressed_key = '5';
            break;
        case Qt::Key_6:
            pressed_key = '6';
            break;
        case Qt::Key_A:
            pressed_key ='a';
            break;
        case Qt::Key_Z:
            pressed_key = 'z';
            break;
        case Qt::Key_Q:
            pressed_key ='q';
            break;
        case Qt::Key_W:
            pressed_key = 'w';
            break;
        case Qt::Key_U:
            pressed_key ='u';
            break;
        case Qt::Key_J:
            pressed_key = 'j';
            break;
        case Qt::Key_I:
            pressed_key ='i';
            break;
        case Qt::Key_K:
            pressed_key = 'k';
            break;
        case Qt::Key_O:
            pressed_key ='o';
            break;
        case Qt::Key_L:
            pressed_key = 'l';
            break;
        case Qt::Key_R:
            pressed_key = 'r';
            break;
        case Qt::Key_Up:
            pressed_key = 65;
            break;
        case Qt::Key_Down:
            pressed_key = 66;
            break;
        case Qt::Key_Right:
            pressed_key = 67;
            break;
        case Qt::Key_Left:
            pressed_key = 68;
            break;
    }


//    QByteArray mKey;
//    mKey = Key->text().toAscii();
//    pressed_key = (char*)mKey.constData();
////    pressed_key = Key->text().toStdString();//.toInt();
////    ui->StatusLbl->setText(pressed_key);//QString::number(pressed_key));
////    ui->StatusLbl->setText(QString::fromStdString(mKey));//QString::number(pressed_key));
//    ui->StatusLbl->setText(QString::fromAscii(mKey));//QString::number(pressed_key));
}
