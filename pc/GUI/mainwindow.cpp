#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QEventLoop>
#include <string.h>
#include <QKeyEvent>

#include "../read_kb.h"
#include "../read_js.h"
#include "../rs232.h"
#include "../Package.h"


int flag = 0;
//int fd_RS232;
BYTE ReadBuffer[6];
char pressed_key = -1;
unsigned char  QRMode = MODE_SAFE; //Initializes QR Mode

void DecodeTelemetry();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    term_nonblocking();
//    QEventLoop(&ui);
    // INITIALIZES CONTROLS
    ui->Lift->display(0);
    ui->Roll->display(0);
    ui->Pitch->display(0);
    ui->Yaw->display(0);
    ui->P->display(0);
    ui->P1->display(0);
    ui->P2->display(0);

    //focus on one of the controls
    ui->label_5->setFocus();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_RunButt_clicked()
{
    //RUN OR STOP DATA TRANSFER
    ui->label_5->setFocus();
    int i = 0;
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

    int nbtx,nbrx;
    while(flag == 0)
    {
        QCoreApplication::processEvents(); //Prevents for GUI Freezing
        read_js(jmap);

        if (pressed_key != -1) {
            //MsgBox(QString::number(pressed_key));

//            usleep(100000);
            //printf("\n Key [%i]",pressed_key);
            read_kb(keymap,&pressed_key);
        }

        //CREATES THE PACKAGE
        SetPkgMode(&mPkg,QRMode);


        QRMode = keymap[0];
        ui->ModeSel->setText(GetMode(QRMode));

        switch (QRMode) {
            case MODE_P:

                data[1] = keymap[5];
                data[2] = keymap[6];
                data[3] = keymap[7];
                data[0] = 0;
                //SETS THE PACKAGE
                SetPkgData(&mPkg,data);
                ui->P->display(data[1]);
                ui->P1->display(data[2]);
                ui->P2->display(data[3]);
                break;
            default: //CONTROL MODES
                //Update Package Data
                data[0] = jmap[0]+keymap[1];
                data[1] = jmap[1]+keymap[2];
                data[2] = jmap[2]+keymap[3];
                data[3] = jmap[3]+keymap[4];
//                printf("\n\n ");
//                for (i = 0; i < 4; i++) {
//                    printf("[%x]",data[i]);
//                }
                //SETS THE PACKAGE
                SetPkgData(&mPkg,data);
                //Update MODE_PSent Info in GUI
                ui->Lift->display(data[0]);
                ui->Roll->display(data[1]);
                ui->Pitch->display(data[2]);
                ui->Yaw->display(data[3]);
                break;
        }


        //Prints the package
//        for (i = 0; i < PKGLEN; i++) {
//            printf("[%x]",(BYTE)data[i]);
//        }
//        printf("\n");

        //SEND PACKAGE HERE
        //writes in the port

        nbtx = write(fd_RS232,mPkg.Pkg,7*sizeof(BYTE));
        //Asserts in case of sending wrong number of bytes
        assert(nbtx == 7);

        //reads from the port
        nbrx = read(fd_RS232, ReadBuffer, 1*sizeof(BYTE));

        if (nbrx > 0){
            //DECODING PART HERE

//            printf("\n\n Read PKG: [%i] bytes",nbrx);
//            for (i = 0; i < nbrx; i++) {
//                printf("[%x]",ReadBuffer[i]);
//            }
//            printf("\n");
            ui->Tel1->display(ReadBuffer[0]);
            ui->Tel2->display(ReadBuffer[1]);
            ui->Tel3->display(ReadBuffer[2]);
            ui->Tel4->display(ReadBuffer[3]);
            ui->Tel5->display(ReadBuffer[4]);
            ui->Tel6->display(ReadBuffer[5]);
        }

       // ui->PkgTxtBox->setText(QString::number(mPkg.ChkSum));

        usleep(10000);

        i++;
    }

//    MsgBox("Test");
}

void DecodeTelemetry(){
//    if (ReadBuffer == 0x80){
//        first_byte = true;
//    }
//    second_byte = false;
//    if (first_byte == true && ReadBuffer == 0x00){
//        second_byte = true;
//    }

//    // STORING DATA
//    if(read_data && datacount < 8){
//        DataTel[datacount] = ReadBuffer;
//        printf("Reading Byte : %i",datacount);
//        datacount++;
//    }

//    // PRINTING RECEIVED DATA
//    if (datacount == 8){
//        printf("\n\nReceived Data:");
//        for (i=0;i<8;i++) printf("[%x]",DataTel[i]);
//    }

//    //
//    if (first_byte == true && second_byte == true){
//        read_data = true;
//        first_byte = false;
//        second_byte = false;
//        printf("\n\n Starting Bytes Received...: ");
//        datacount = 0;
//    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label_5->setFocus();
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
    ui->label_5->setFocus();
    if (ui->CommButt->text() == "Connect"){
        //OPEN HERE COM PORT
        ui->StatusLbl->setText("Clearing Joystick Buffer...");
        clear_js_buffer();
        ui->StatusLbl->setText("Opening Port...");
        //fd_RS232 = open_rs232_port();
        rs232_open();
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
        //close_rs232_port(fd_RS232);
        ui->StatusLbl->setText("Port Closed!");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : white}");
        ui->CommButt->setText("Connect");
        flag = 1;
    }
    ui->label_5->setFocus();
}


void MainWindow::keyPressEvent(QKeyEvent* Key)
{
//    ui->StatusLbl->setText(Key->text().toLower());
//    ui->StatusLbl->setText(QString::number(Key->key()));
    switch (Key->key()){
        case Qt::Key_0:
            pressed_key = 48;
            break;
        case Qt::Key_1:
            pressed_key = 49;
            break;
        case Qt::Key_2:
            pressed_key = 50;
            break;
        case Qt::Key_3:
            pressed_key = 51;
            break;
        case Qt::Key_4:
            pressed_key = 52;
            break;
        case Qt::Key_5:
            pressed_key = 53;
            break;
        case Qt::Key_6:
            pressed_key = 54;
            break;
        case Qt::Key_A:
            pressed_key = 97;
            break;
        case Qt::Key_Z:
            pressed_key = 122;
            break;
        case Qt::Key_Q:
            pressed_key = 113;
            break;
        case Qt::Key_W:
            pressed_key = 119;
            break;
        case Qt::Key_U:
            pressed_key = 117;
            break;
        case Qt::Key_J:
            pressed_key = 106;
            break;
        case Qt::Key_I:
            pressed_key = 105;
            break;
        case Qt::Key_K:
            pressed_key = 107;
            break;
        case Qt::Key_O:
            pressed_key = 111;
//
            break;
        case Qt::Key_L:
            pressed_key = 108;
            break;
        case Qt::Key_R:
            pressed_key = 114;
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
    ui->StatusLbl->setText(QString::number(pressed_key));

//    QByteArray mKey;
//    mKey = Key->text().toAscii();
//    pressed_key = (char*)mKey.constData();
////    pressed_key = Key->text().toStdString();//.toInt();
////    ui->StatusLbl->setText(pressed_key);//QString::number(pressed_key));
////    ui->StatusLbl->setText(QString::fromStdString(mKey));//QString::number(pressed_key));
//    ui->StatusLbl->setText(QString::fromAscii(mKey));//QString::number(pressed_key));
}
