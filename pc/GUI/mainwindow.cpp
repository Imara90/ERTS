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

//#define FALSE 	0
//#define TRUE 	1

#define START_BYTE 0x80
#define TELPKGLEN     6 //EXPECTED TELEMETRY PACKAGE LENGTH EXCLUDING THE STARTING BYTE
#define TELPKGCHKSUM  TELPKGLEN - 1

#define START_BYTE 0x80
#define DLPKGLEN     6 //EXPECTED DATA LOG PACKAGE LENGTH EXCLUDING THE STARTING BYTE
#define DLPKGCHKSUM  DLPKGLEN - 1

//Flag to trigger data logging
bool DataLogkey = false;


int TeleDecode(int* TelPkg/*, int* Output*/){

    int i;
    int sum = 0;
    int ChkSum = TelPkg[TELPKGCHKSUM];
    //CHECKSUM CHECK
    for(i = 0; i < TELPKGCHKSUM ; i++)
    {
        sum += TelPkg[i];
    }
    sum = (BYTE)~sum;
//	printf("[%x][%x]",,ChkSum);
    if (ChkSum == sum)
    {
        //DECODING PART
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

int DLDecode(int* DLPkg/*, int* Output*/){

    int i;
    int sum = 0;
    int ChkSum = DLPkg[DLPKGCHKSUM];
    //CHECKSUM CHECK
    for(i = 0; i < DLPKGCHKSUM ; i++)
    {
        sum += DLPkg[i];
    }
    sum = (BYTE)~sum;
//	printf("[%x][%x]",,ChkSum);
    if (ChkSum == sum)
    {
        //DECODING PART
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


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


    //----------------------------------- VARIABLES INITIALIZATION-------------------------------//
    //Initializes the keymap from the keyboard
    int keymap[8] = {MODE_SAFE,0,0,0,0,0,0,0};
    //Initializes the keymap from the keyboard
    int jmap[4] = {0,0,0,0};
    //Joystick buffer clearence and calibration of yaw axis
//	clear_js_buffer();
//	js_calibration();

    /*Initializes the Package Data (Lift,Roll,Pitch,Yaw for Control Modes)
     *(P,P1,P2,0 for Control Gains Mode)*/
    int data[PARAM_LENGTH] = {0,0,0,0};
    Package mPkg;
    InitPkg(&mPkg,MODE_SAFE); //Intializes Package

    //General Counter
    int i;
    //AbortFlag
    int abort = 0;
    //Stores the read byte
    BYTE readbuff;
    //Count variable which loops over the sending package
    int datai = 0;
    //Used to check whether the complete Pkg has been written
    int Pkg_written = 0;
    //Used to disable writting to the port
    int writeflag = 1;
    //Used to loop over the read Pkg
    int datacount  = 0;
    // Variables supervision for tx and rx number of sent/received bytes
    int 	nbrx, nbtx;
    nbtx=nbrx=0;

    //--------------------------DATA LOGGING FILE----------------------------------------------------//
    int DLData[TELPKGLEN];
    // Intializes DataLogging Pkg
    for(i = 0; i < TELPKGLEN; i++){
        DLData[i] = 0;
    }
    //Used to request data logging
    int DLreq = 0;
    FILE *DLfile = fopen("DATALOGGING.txt", "w");
    if (DLfile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //-------------------------------TELEMETRY-----------------------------------------------------------//
    int TeleData[TELPKGLEN];
    int ChkSumOK = FALSE;
    //Initializes telemetry array
    for(i = 0; i < TELPKGLEN; i++){
        TeleData[i] = 0;
    }
    int DisplayArray[7];// Contains the current mode along with the 6 sensors values
    //Initializes display array
    for(i = 0; i < 7; i++){
        DisplayArray[i] = 0;
    }
    //Stores Telemetry Data
    FILE *TeleFile = fopen("Telemetry.txt", "w");
    if (TeleFile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    //------------------------------------GUI PARAMETERS----------------------
    //RUN OR STOP DATA TRANSFER
    ui->label_5->setFocus();

    if (ui->RunButt->text() == "Run"){
        //Start communication
        ui->StatusLbl->setText("Running...");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : orange}");
        ui->RunButt->setText("Stop");
        ui->CommButt->setEnabled(false);
        ui->SaveDL->setEnabled(true);
        flag = 0;
    }
    else
    {
        // Closes the Files
        fclose(DLfile);
        fclose(TeleFile);
        //Stops communication
        ui->StatusLbl->setText("Stopped");
        ui->StatusLbl->setStyleSheet("QLabel { background-color : white}");
        ui->RunButt->setText("Run");
        ui->CommButt->setEnabled(true);
        ui->SaveDL->setEnabled(false);
        flag = 1;
    }

    while(flag == 0)
    {
        QCoreApplication::processEvents(); //Prevents for GUI Freezing

        //reads data from the joystick ...comment if joystick is not connected
        //abort = read_js(jmap);
        //CHECKS KEYBOARD INPUT FOR DATALOGGING
        if (DataLogkey == true){ //Data Logging requested
            pressed_key = 126;
            DLreq = 1;
            datacount = 0; // Resets the counter (Reading new data)
        }
        //KEYBOARD MAP
        if (pressed_key != -1) {
            //Creates the keyboard map based on the pressed key
            read_kb(keymap,&pressed_key);
        }
        //EVALUATES IF ABORTION REQUESTED
        if (abort == 1) keymap[0] = MODE_ABORT;
        QRMode = keymap[0];
        ui->ModeSel->setText(GetMode(QRMode));
        switch (QRMode) {
            case MODE_P:

                data[1] = keymap[5];
                data[2] = keymap[6];
                data[3] = keymap[7];
                data[0] = 0;
                //SETS THE PACKAGE
                SetPkgMode(&mPkg,QRMode);
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
                //SETS THE PACKAGE
                SetPkgMode(&mPkg,QRMode);
                SetPkgData(&mPkg,data);
                //Update MODE_PSent Info in GUI
                ui->Lift->display(data[0]);
                ui->Roll->display(data[1]);
                ui->Pitch->display(data[2]);
                ui->Yaw->display(data[3]);
                break;
        }

        if (writeflag == 1){
            //WRITTING
            //Writes the pkg byte by byte. Makes sure that each byte is written
            do{
                nbtx = write(fd_RS232, &(mPkg.Pkg[datai]), sizeof(BYTE));
                if (nbtx == 1) { //a Byte has been written
                    datai++;
                }
                if (datai >= PKGLEN){ //The Pkg has been completely written
                    datai = 0;
                    Pkg_written = 1;
                }
                else {//The Pkg has NOT been completely written
                    Pkg_written = 0;
                }
            }while(Pkg_written == 0);
        }
        //READING
        //Makes sure everything is read from the RX line
        do{
            nbrx = read(fd_RS232, &readbuff, sizeof(BYTE));
            //printf("\n [%i] %i nbrx = %i",writeflag,buff_count++,nbrx);
            if (nbrx > 0)
            {
                // CHECKING FOR STARTING BYTE
                if (readbuff == START_BYTE)
                {
                    datacount = 0; //Reset the data counter. Starts over!!
                }
                // STORING THE DATA. Starts over if a starting BYTE is found!!!
                if (readbuff != START_BYTE)
                {
                    TeleData[datacount] = readbuff;
                    datacount++;
                }
                // NORMAL OPERATION
                if (DLreq == 0){
                    // TELEMETRY DECODING. Only If the store data has the expected size
                    if (datacount == TELPKGLEN) //Complete Pkg Received
                    {
                        //Prints the stored package
                        /*for (i = 0; i < TELPKGLEN; i++) {
                            printf("[%x]",TeleData[i]);
                        }*/

                        //DISPLAYS THE DATA IN THE GUI
                        ui->Tel1->display(TeleData[0]);
                        ui->Tel2->display(TeleData[1]);
                        ui->Tel3->display(TeleData[2]);
                        ui->Tel4->display(TeleData[3]);
                        ui->Tel5->display(TeleData[4]);
                        ui->Tel6->display(TeleData[5]);


                        // DECODING. Checksum proof and stores decoded values in new array DispData
                        //ChkSumOK = decode(TeleData,&DispData);
                        ChkSumOK = TeleDecode(TeleData);
                        //printf(" Chksum OK = %i \n",ChkSumOK);
                        //Saves data only if the pkg is complete
                        if (ChkSumOK){
                            //Writes the telemetry in a Txt file
                            for (i = 0; i < TELPKGLEN; i++) {
                                fprintf(TeleFile, "%x ", TeleData[i]);
                            }
                            fprintf(TeleFile,"\n");
                        }
                    }
                }
                else //DATA LOG REQUESTED Shuts writting down and only reads
                {
                    // DATA LOGGING DECODING. Only If the stored data has the expected size
                    if (datacount == DLPKGLEN) //Complete Pkg Received
                    {
                        /*printf("DL ");
                        //Prints the stored package
                        for (i = 0; i < DLPKGLEN; i++) {
                            printf("[%x]",TeleData[i]);
                        }*/
                        //DECODING. Checksum proof and stores decoded values in new array DispData
                        //ChkSumOK = decode(TeleData,&DispData);
                        ChkSumOK = DLDecode(TeleData);
                        //printf(" Chksum OK = %i \n",ChkSumOK);
                        //Saves data only if the pkg is complete
                        if (ChkSumOK){
                            //Writes the datalog in a Txt file
                            for (i = 0; i < DLPKGLEN; i++) {
                                fprintf(DLfile, "%x ", TeleData[i]);
                            }
                            fprintf(DLfile,"\n");
                        }
                    }
                    writeflag == 0; //Disables writting
                }
                //dltimeout = 0;
            }
        } while (nbrx > 0);
        pressed_key = -1;
    }
    DataLogkey = false;
//    MsgBox("Test");
}




void MainWindow::on_CommButt_clicked()
{
    ui->label_5->setFocus();
    if (ui->CommButt->text() == "Connect"){
        //OPEN HERE COM PORT
        ui->StatusLbl->setText("Clearing Joystick Buffer...");
        //clear_js_buffer();
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
        case Qt::Key_Delete:
            pressed_key = 126;
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

void MainWindow::on_SaveDL_clicked()
{
    DataLogkey = true;
}
