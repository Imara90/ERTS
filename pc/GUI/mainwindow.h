#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "../Definitions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString GetMode(unsigned char QRMode){
    switch (QRMode){
        case MODE_SAFE:
            return QString("SAFE MODE");
            break;
        case MODE_PANIC:
            return QString("PANIC MODE");
            break;
        case MODE_MANUAL:
            return QString("MANUAL MODE");
            break;
        case MODE_CALIBRATION:
            return QString("CALIBRATION MODE");
            break;
        case MODE_YAW_CONTROL:
            return QString("YAW CONTROL MODE");
            break;
        case MODE_FULL_CONTROL:
            return QString("FULL CONTROL MODE");
            break;
        case MODE_P:
            return QString("P MODE");
            break;
        case MODE_ABORT:
            return QString("ABORT MODE");
            break;
        default:
            return QString("SAFE MODE");
            break;
        }
    }

    void MsgBox(QString Text){
        QMessageBox mBox;
        mBox.setText(Text);
        mBox.exec();
    }



private slots:
    void on_RunButt_clicked();

    void on_CommButt_clicked();

    void keyPressEvent(QKeyEvent* Key);

    void on_SaveDL_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
