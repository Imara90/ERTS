#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MsgBox(QString Text){
        QMessageBox mBox;
        mBox.setText(Text);
        mBox.exec();
    }



private slots:
    void on_RunButt_clicked();

    void on_pushButton_2_clicked();

    void on_ModeSel_currentIndexChanged(int index);

    void on_CommButt_clicked();

    void keyPressEvent(QKeyEvent* Key);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
