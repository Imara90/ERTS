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
#include "../mode_selection.h" 	// Diogos mode selection function
#include "../manual_mode_pc.h"

int flag = 0;
//int fd_RS232;
BYTE ReadBuffer[6];
char pressed_key = -1;
unsigned char  QRMode = MODE_SAFE; //Initializes QR Mode

//#define FALSE 	0
//#define TRUE 	1

#define START_BYTE 0x80
#define TELLEN	      	8
#define TELPKGLEN     	TELLEN - 1
#define TELPKGCHKSUM  	TELPKGLEN - 1

#define START_BYTE 0x80
#define DATALEN         48
#define DLPKGLEN     	DATALEN - 1 //EXPECTED DATA LOG PACKAGE LENGTH EXCLUDING THE STARTING BYTE
#define DLPKGCHKSUM  	DLPKGLEN - 1

//Flag to trigger data logging
bool DataLogkey = false;

FILE* DLfile;
FILE* TeleFile;
int dataclose = -1;
int telclose = -1;

/***************************************************************/
//	Checks and decodes telemetry packets
// Author: Daniel Lemus 870754
/***************************************************************/
int TeleDecode(int* TelPkg/*, int* Output*/){

    int i;
    int sum = 0;
    int ChkSum = TelPkg[TELPKGCHKSUM];
    //CHECKSUM CHECK
    for(i = 0; i < TELPKGCHKSUM ; i++)
    {
        sum ^= TelPkg[i];
    }
    sum = (BYTE)~sum;
    if (sum == 0x80)
    {
            sum = 0x00;
        }
    // DEBUG
    //sumglobal = sum;
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

/***************************************************************/
//	Checks and DataLog packets
// Author: Daniel Lemus 870754
/***************************************************************/
int DLDecode(int* DLPkg/*, int* Output*/){

    int i;
    int sum = 0;
    int ChkSum = DLPkg[DLPKGCHKSUM];
    //CHECKSUM CHECK
    for(i = 0; i < DLPKGCHKSUM ; i++)
    {
        sum ^= DLPkg[i];
    }
    sum = (BYTE)~sum;
        if (sum == 0x80)
    {
            sum = 0x00;
        }
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

/***************************************************************/
/*	Sets status bar text and color
/* Author: Daniel Lemus 870754
/***************************************************************/
void SetStatusColor(QStatusBar* statusBar,QString mString,QColor mColor){
    QPalette palette;
    palette.setColor(QPalette::Background,mColor);
    statusBar->showMessage(mString);
    statusBar->setPalette(palette);
}


/***************************************************************/
//	Initializes Main Window
// Author: Daniel Lemus 870754
/***************************************************************/

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

/***************************************************************/
//	Triggers Main Window destructor
// Author: Daniel Lemus 870754
/***************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}

/***************************************************************/
//	Run button clicks
// Author: Daniel Lemus 870754
/***************************************************************/
void MainWindow::on_RunButt_clicked()
{
    //------------------------------------GUI PARAMETERS----------------------
    //RUN OR STOP DATA TRANSFER
    ui->label_5->setFocus();

    if (ui->RunButt->text() == "Start Sending..."){
        //Start communication
        SetStatusColor(ui->statusBar,"Running... ",Qt::yellow);
        ui->CommButt->setEnabled(false);
        ui->SaveDL->setEnabled(true);
        flag = 0;
        ui->SaveDL->setEnabled(true);
        ui->RunButt->setText("Stop Sending...");
    }
    else
    {
        if (QRMode == MODE_SAFE || QRMode == MODE_ABORT){
//            // Closes the Files
//            if (telclose != 0);
//            fclose(DLfile);
//            if (telclose != 0);
//            fclose(TeleFile);
            //Stops communication
            SetStatusColor(ui->statusBar,"Stopped... !!!",Qt::yellow);
            //ui->StatusLbl->setStyleSheet("QLabel { background-color : white}");
            ui->RunButt->setText("Start Sending...");
            ui->CommButt->setEnabled(true);
            ui->SaveDL->setEnabled(false);
            flag = 1;
        }else{
            SetStatusColor(ui->statusBar,"Warning!! Please set SAFE MODE before stopping communication!!",Qt::red);
            return;
        }
    }


    //----------------------------------- VARIABLES INITIALIZATION-------------------------------//
    //Engine Values for mode selection
    int ae[4]={0,0,0,0};
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

    // Variables supervision for tx and rx number of sent/received bytes
    int 	nbrx, nbtx;
    nbtx=nbrx=0;

    //DATA LOGGING FILE
    int linecount=0;
    int DLData[DLPKGLEN];
    for(i = 0; i < DLPKGLEN; i++){
        DLData[i] = 0;
    }
    int dltimeout = 0;
    //Used to request data logging
    int DLreq = 0;
    DLfile = fopen("DATALOGGING.txt", "w");
    if (DLfile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    dataclose = -1;
    //TELEMETRY
    int TeleData[TELPKGLEN];
    //Used to loop over the read Pkg
    int datacount  = 0;
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
    TeleFile = fopen("Telemetry.txt", "w");
    if (TeleFile == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    telclose = -1;

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

        if (writeflag == 1){

            //KEYBOARD MAP
            if (pressed_key != -1) {
                //Creates the keyboard map based on the pressed key
                read_kb(keymap,&pressed_key);
            }
            //EVALUATES IF ABORTION REQUESTED
            if (abort == 1) QRMode = keymap[0] = MODE_ABORT;
            QRMode = keymap[0];
            ui->ModeSel->setText(GetMode(QRMode));

            switch (keymap[0]) {
                case MODE_P: //CONTROL GAINS, Starting from the second place in data array (First place is reserved for lift value)
                    data[0] = 0;
                    data[1] = keymap[5];
                    data[2] = keymap[6];
                    data[3] = keymap[7];
                    ui->P->display(data[1]);
                    ui->P1->display(data[2]);
                    ui->P2->display(data[3]);
                    break;

                default: //CONTROL MODES
                    data[0] = jmap[0]+keymap[1];
                    data[1] = jmap[1]+keymap[2];
                    data[2] = jmap[2]+keymap[3];
                    data[3] = jmap[3]+keymap[4];
                    //Update MODE_PSent Info in GUI
                    ui->Lift->display(data[0]);
                    ui->Roll->display(data[1]);
                    ui->Pitch->display(data[2]);
                    ui->Yaw->display(data[3]);
                    break;
            }
            switch (keymap[0])
            {
                case MODE_MANUAL:
                    manual_mode(data[0], data[1], data[2], data[3],ae);
                    break;
                case MODE_YAW_CONTROL:
						  yaw_control_mode_pc(data[0], data[1], data[2], data[3],ae);
                    break;
                case MODE_FULL_CONTROL:
						  full_control_mode_pc(data[0], data[1], data[2], data[3],ae);
                    break;
                default:
                    for(i = 0;i<4;i++)
                    {
                        ae[i] = 0;
                    }
                    break;
            }
            //DISPLAY THE ENGINE VALUES
            ui->ae0->display(ae[0]);
            ui->ae1->display(ae[1]);
            ui->ae2->display(ae[2]);
            ui->ae3->display(ae[3]);
            //MODE SELECTION
            mode_selection(keymap,data[0]);
            QRMode = keymap[0];
            //SETS THE PACKAGE WITH THE DESIRED DATA
            SetPkgMode(&mPkg, keymap[0]);
            SetPkgData(&mPkg, data);
            //Prints the package
            /*for (i = 0; i < PKGLEN; i++) {
                printf("[%x]",mPkg.Pkg[i]);
            }
            printf("\n");*/

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

                // NORMAL OPERATION
                if (DLreq == 0){
                    // STORING THE DATA. Starts over if a starting BYTE is found!!!
                    if (readbuff != START_BYTE)
                    {
                        TeleData[datacount] = readbuff;
                        //DLData[datacount] = readbuff;
                        datacount++;
                    }
                    // TELEMETRY DECODING. Only If the store data has the expected size
                    if (datacount == TELPKGLEN) //Complete Pkg Received
                    {

                        // using the telemetry for mode switching
                        TELEMETRY_FLAG = TeleData[TELPKGLEN - 2];
                        //CHECKS WHETHER CALIBRATION IS DONE
                        if(QRMode == MODE_CALIBRATION){
                            if(TELEMETRY_FLAG & 0x01){
                                SetStatusColor(ui->statusBar,"Calibration has been sucessful!!",Qt::green);
                            }else
                            {
                                SetStatusColor(ui->statusBar,"Calibration in progress",Qt::yellow);
                            }
                        }
                        // DECODING. Checksum proof and stores decoded values in new array DispData
                        ChkSumOK = TeleDecode(TeleData);
//                                printf(" Chksum OK = %i \n",ChkSumOK);
                        //Saves data only if the pkg is complete
                        if (ChkSumOK){
                            ui->r_qr->display((int)TeleData[0]);
                            ui->phi_qr->display((TeleData[1] << 8 | TeleData[2]));
                            ui->theta_ref->display((TeleData[3] << 8 | TeleData[4]));
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
                    // STORING THE DATA. Starts over if a starting BYTE is found!!!
                    SetStatusColor(ui->statusBar,"Downloading DataLog. Please wait...",Qt::yellow);
                    ui->SaveDL->setEnabled(false);
                    ui->RunButt->setEnabled(false);
                    if (readbuff != START_BYTE)
                    {
                        DLData[datacount] = readbuff;
                        datacount++;
                    }
                    // DATA LOGGING DECODING. Only If the stored data has the expected size
                    if (datacount == DLPKGLEN) //Complete Pkg Received
                    {
                        datacount = 0;
//                        if (abort == 1) keymap[0] = MODE_ABORT;
//                        QRMode = keymap[0];
//                        ui->ModeSel->setText(GetMode(QRMode));
//                        printf("DL ");
                        //Prints the stored package
//                        for (i = 0; i < DLPKGLEN; i++) {
//                            printf("[%x]",DLData[i]);
//                        }
                        //printf("    [%x]", sumglobal);
                        //DECODING. Checksum proof and stores decoded values in new array DispData
                        //ChkSumOK = decode(TeleData,&DispData);
                        ChkSumOK = DLDecode(DLData);
//                        printf(" Chksum OK = %i \n",ChkSumOK);
                        //Saves data only if the pkg is complete
                        if (ChkSumOK){
                            //Writes the datalog in a Txt file
                            for (i = 0; i < DLPKGLEN; i++) {
                                fprintf(DLfile, "%i ", DLData[i]);
                            }
                            SetStatusColor(ui->statusBar,QString::number(linecount++),Qt::yellow);
                            fprintf(DLfile,"\n");
                        }
                    }
                    writeflag = 0;
                }
                dltimeout = 0;
            }
        } while (nbrx > 0);

        if( (dltimeout++ > 200000) && writeflag == 0){
            rs232_close();
            SetStatusColor(ui->statusBar,"Data Login Downloaded...",Qt::yellow);
            sleep(1);
            telclose = fclose(TeleFile);
            if (telclose == 0)
            {
                SetStatusColor(ui->statusBar,"Telemetry file closed correctly",Qt::green);
            }
            else SetStatusColor(ui->statusBar,"Telemetry file closed wrong",Qt::red);
            sleep(1);
            dataclose = fclose(DLfile);
            if (dataclose == 0)
            {
                SetStatusColor(ui->statusBar,"\nDatalog closed correctly",Qt::green);
            }
            else SetStatusColor(ui->statusBar,"\nDatalog closed wrong",Qt::red);
            sleep(1);
            DataLogkey = false;
            ui->RunButt->setEnabled(true);
            return;
        }

        pressed_key = -1;
    }
    DataLogkey = false;
//    MsgBox("Test");
}



/***************************************************************/
//	Handles Communication button clicks
// Author: Daniel Lemus 870754
/***************************************************************/
void MainWindow::on_CommButt_clicked()
{
    ui->label_5->setFocus();
    if (ui->CommButt->text() == "Connect"){
        //OPEN HERE COM PORT
        SetStatusColor(ui->statusBar,"Clearing Joystick Buffer...",Qt::yellow);
        //clear_js_buffer();
        SetStatusColor(ui->statusBar,"Opening Port...",Qt::yellow);
        //fd_RS232 = open_rs232_port();
        rs232_open();
        if (fd_RS232 == 0)
        {

            SetStatusColor(ui->statusBar,"Error opening Port...",Qt::red);
            return;
        }
        ui->RunButt->setEnabled(1);
        SetStatusColor(ui->statusBar,"Connected",Qt::green);

        ui->CommButt->setText("Disconnect");
        flag = 0;
    }
    else
    {
        //CLOSE HERE PORT
        ui->RunButt->setEnabled(0);
        SetStatusColor(ui->statusBar,"Closing the Port...",Qt::yellow);
        //close_rs232_port(fd_RS232);
        SetStatusColor(ui->statusBar,"Port Closed!",Qt::green);
        //ui->StatusLbl->setStyleSheet("QLabel { background-color : white}");
        ui->CommButt->setText("Connect");
        flag = 1;
    }
    ui->label_5->setFocus();
}

/***************************************************************/
//	Handles pressed keys from the keyboard
// Author: Daniel Lemus 870754
/***************************************************************/
void MainWindow::keyPressEvent(QKeyEvent* Key)
{
//    ui->statusBar->showMessage(Key->text().toLower());
//    ui->statusBar->showMessage(QString::number(Key->key()));
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
    //ui->statusBar->showMessage(QString::number(pressed_key));
}

/***************************************************************/
//	Handles Save Data Log Key
// Author: Daniel Lemus 870754
/***************************************************************/
void MainWindow::on_SaveDL_clicked()
{
    DataLogkey = true;
}

/***************************************************************/
//	Handles main window close event
// Author: Daniel Lemus 870754
/***************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
    SetStatusColor(ui->statusBar,"Closing ....",Qt::yellow);
    if(ui->RunButt->text() == "Stop Sending..."){
        SetStatusColor(ui->statusBar,"Stopping communication with QR ....",Qt::yellow);
        ui->RunButt->click();
        sleep(1);
        SetStatusColor(ui->statusBar,"Closing the port ....",Qt::yellow);
        ui->CommButt->click();
    }
}
