#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    // List of signals and the appropriate slot that they should connect to
    connect(ui->fetchButton, SIGNAL (pressed()), this, SLOT (fetchPressed()));
    connect(ui->fetchButton, SIGNAL (hovered()), this, SLOT (fetchPressed()));

    connect(ui->settingButton, SIGNAL (pressed()), this, SLOT (settingsPressed()));
    connect(ui->settingButton, SIGNAL (hovered()), this, SLOT (settingsPressed()));

    connect(ui->tutorialButton, SIGNAL (pressed()), this, SLOT (tutorialPressed()));
    connect(ui->tutorialButton, SIGNAL (hovered()), this, SLOT (tutorialPressed()));

    connect(ui->backButton, SIGNAL (pressed()), this, SLOT (backPressed()));
    connect(ui->backButton, SIGNAL (hovered()), this, SLOT (backPressed()));
    connect(ui->backButton_2, SIGNAL (pressed()), this, SLOT (backPressed()));
    connect(ui->backButton_2, SIGNAL (hovered()), this, SLOT (backPressed()));
    connect(ui->backButton_3, SIGNAL (pressed()), this, SLOT (backPressed()));
    connect(ui->backButton_3, SIGNAL (hovered()), this, SLOT (backPressed()));

    connect(ui->tutorialButton, SIGNAL (pressed()), this, SLOT (tutorialPressed()));
    connect(ui->tutorialButton, SIGNAL (hovered()), this, SLOT (tutorialPressed()));

    connect(ui->hover_time_up_button, SIGNAL (pressed()), this, SLOT (hover_time_up()));
    connect(ui->hover_time_up_button, SIGNAL (hovered()), this, SLOT (hover_time_up()));

    connect(ui->hover_time_down_button, SIGNAL (pressed()), this, SLOT (hover_time_down()));
    connect(ui->hover_time_down_button, SIGNAL (hovered()), this, SLOT (hover_time_down()));

    connect(ui->hoverButton, SIGNAL (changeLabel()), this, SLOT (changeLabel()));

    connect(ui->upButton, SIGNAL (pressed()), this, SLOT (move_up()));
    connect(ui->upButton, SIGNAL (hovered()), this, SLOT (move_up()));

    connect(ui->downButton, SIGNAL (pressed()), this, SLOT (move_down()));
    connect(ui->downButton, SIGNAL (hovered()), this, SLOT (move_down()));

    connect(ui->leftButton, SIGNAL (pressed()), this, SLOT (move_left()));
    connect(ui->leftButton, SIGNAL (hovered()), this, SLOT (move_left()));

    connect(ui->rightButton, SIGNAL (pressed()), this, SLOT (move_right()));
    connect(ui->rightButton, SIGNAL (hovered()), this, SLOT (move_right()));

    connect(ui->forwardButton, SIGNAL (pressed()), this, SLOT (move_forward()));
    connect(ui->forwardButton, SIGNAL (hovered()), this, SLOT (move_forward()));

    connect(ui->backwardButton, SIGNAL (pressed()), this, SLOT (move_backward()));
    connect(ui->backwardButton, SIGNAL (hovered()), this, SLOT (move_backward()));

    connect(ui->releaseButton, SIGNAL (pressed()), this, SLOT (move_finished()));
    connect(ui->releaseButton, SIGNAL (hovered()), this, SLOT (move_finished()));

    //Open serial port
    port.setPortName("/dev/cu.usbmodem1421");
    port.setBaudRate(QSerialPort::Baud9600);
    port.setDataBits(QSerialPort::Data8);
    port.setParity(QSerialPort::NoParity);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);

    port.open(QIODevice::ReadWrite);

    if (port.isOpen()){
        qDebug() << "OPEN!!!";
    }
    else{
        qDebug() << "Serial port could not open";
        qDebug() << port.errorString();
    }

}


MainWindow::~MainWindow()
{
    if (port.isOpen()){
        port.close();
    }
    delete ui;
}

void MainWindow::write_to_arduino(char data){
    if (!port.isOpen()){
        qDebug() << "ERROR! PORT NOT OPEN!";
        return;
    }

    ard_data.clear();
    ard_data.push_back(data);
    port.write(ard_data);
}

void MainWindow::fetchPressed(){
    ui->stackedWidget->setCurrentIndex(1);
    Command = 'E';
    write_to_arduino(Command);
}

void MainWindow::tutorialPressed(){
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::settingsPressed(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::backPressed(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::hover_time_down(){

    QHoverSensitiveButton::hoverTime -= 1000;

    if (QHoverSensitiveButton::hoverTime <= 0){
        QHoverSensitiveButton::hoverTime = 0;
    }
    QString display = QString::number(QHoverSensitiveButton::hoverTime / 1000) + " secs";
    ui->hover_time_label->setText(display);
}

void MainWindow::hover_time_up(){
    QHoverSensitiveButton::hoverTime += 1000;

    QString display = QString::number(QHoverSensitiveButton::hoverTime / 1000) + " secs";
    ui->hover_time_label->setText(display);
}

void MainWindow::changeLabel(){
    if (ui->label->text() == "Hover + Hold"){
        ui->label->setText("Press Button");
    }
    else{
        ui->label->setText("Hover + Hold");
    }
}

void MainWindow::move_up(){
    qDebug() << "Move up";
    Command = 'U';
    write_to_arduino(Command);
}
void MainWindow::move_down(){
    qDebug() << "Move down";
    Command = 'D';
    write_to_arduino(Command);
}
void MainWindow::move_left(){
    qDebug() << "Move left";
    Command = 'L';
    write_to_arduino(Command);
}
void MainWindow::move_right(){
    qDebug() << "Move right";
    Command = 'R';
    write_to_arduino(Command);
}
void MainWindow::move_forward(){
    qDebug() << "Move forward";
    Command = 'F';
    write_to_arduino(Command);
}
void MainWindow::move_backward(){
    qDebug() << "Move backward";
    Command = 'B';
    write_to_arduino(Command);
}

void MainWindow::move_finished(){
    qDebug() << "Retract";
    Command = 'S';

    write_to_arduino(Command);

    QHoverSensitiveButton::t.setHMS(-1,-1,-1,-1);
    QHoverSensitiveButton::hoverButton = "";

    Release popup; // TODO: move code out of setup function
    popup.setVisible(true);
    popup.setModal(true);
    popup.open();
    qDebug() << "opening";
    popup.countdown();

    delay(6000);
    qDebug() << "closing";
    popup.done(1);
}
