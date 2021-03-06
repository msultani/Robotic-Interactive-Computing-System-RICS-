#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::popup_open = false;

int MainWindow::x_pos = 93;
int MainWindow::y_pos = 40;
int MainWindow::z_pos = 40;
int MainWindow::move_speed = 5;
bool MainWindow::auto_movement= true;
bool MainWindow::voice_command_given = false;
QByteArray MainWindow::TCP_data = "";

/* X SERVO: 0
 * Y SERVO: 1
 * Z SERVO: 2
 * CLAW SERVO: 3
 * RETRIEVE: 4
 * STOP: 5
 */

//PYTHON COMMANDS: fetch, up, down, left, right, forward, backward, stop


void MainWindow::establish_TCP_connection(){
    qDebug() << "Establishing TCP connection";
    t = new QTcpServer( this );
    connect(t, SIGNAL (newConnection()), this, SLOT (connection()));

    if (!t->listen(QHostAddress::Any, 6000)){
        qDebug() << "Server did not start";
    }
    else{
        qDebug() << "TCP connected";
    }

}

void MainWindow::connection(){
    qDebug() << "Establishing connection";
    sock = t->nextPendingConnection();

    connect(sock, SIGNAL(readyRead()), this, SLOT(readTCPData()));

    //qDebug() << "closing socket";
    //sock->close();
}

void MainWindow::readTCPData(){
    qDebug() << "here";
    TCP_data = this->sock->readAll();
    qDebug() << "TCP command is:" << TCP_data;
    voice_command_given = true;
    parse_TCP_command(TCP_data);

}

void MainWindow::parse_TCP_command(QByteArray TCP_data){

    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << TCP_data;

    switch(voice_commands.indexOf(TCP_data)){
        case 0:
            fetchPressed();
            break;
        case 1:
            move_up();
            break;
        case 2:
            move_down();
            break;
        case 3:
            move_left();
            break;
        case 4:
            move_right();
            break;
        case 5:
            move_forward();
            break;
        case 6:
            move_backward();
        default:
            qDebug() << "This shouldn't be called... something went wrong";
            break;
    }

    /*
     * Working code for the "stop" method
     * Reads in a voice command, continues to (slowly) move the arm
     * until "stop" command is received
     * Need to ensure that this method can be implemented safely - waiting for Omega
    while (voice_command_given){
        qDebug() << "entering while loop";
        switch(voice_commands.indexOf(TCP_data)){
            case 0:
                fetchPressed();
                break;
            case 1:
                move_up();
                break;
            case 2:
                move_down();
                break;
            case 3:
                move_left();
                break;
            case 4:
                move_right();
                break;
            case 5:
                move_forward();
                break;
            case 6:
                move_backward();
            case 7:
                voice_command_given = false;
                qDebug() << "Stopping";
                break;
            default:
                qDebug() << "This shouldn't be called... something went wrong";
                break;
        }
        delay(2000);
    }
*/

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    //Initialize voice_commands list
    voice_commands << "fetch" << "up" << "down" << "left" << "right" << "forward" << "backward";

    establish_TCP_connection();

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
    //connect(ui->hoverButton, SIGNAL (pressed()), this, SLOT (hover_pressed()));

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

    connect(ui->autoButton, SIGNAL (pressed()), this, SLOT (auto_move()));
    connect(ui->autoButton, SIGNAL (hovered()), this, SLOT (auto_move()));

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
    this->sock->close();
    delete ui;
}

void MainWindow::write_to_arduino(QString data){
    if (!port.isOpen()){
        qDebug() << "ERROR! PORT NOT OPEN!";
        return;
    }

    //ard_data.clear();
    //ard_data.push_back(data);
    const char* myChar = data.toStdString().c_str();
    //ard_data.push_back(myChar);
    port.write(myChar, data.length());

}

void MainWindow::fetchPressed(){
    bool changed = check_hovermode();

    ui->stackedWidget->setCurrentIndex(1);
    if (auto_movement){
        x_pos = 10;
        y_pos = 0;
        z_pos = 0;
        write_to_arduino("0" + QString::number(x_pos) + "X");
        write_to_arduino("1" + QString::number(y_pos) + "X");
        write_to_arduino("2" + QString::number(z_pos) + "X");
    }

    if (changed){
        qDebug() << "CHANGING HOVERMODE";
        ui->ready_label->setText("Please wait.");
        delay(4000);
        QHoverSensitiveButton::hoverMode = true;
        qDebug() << "RESTORING HOVERMODE";
        ui->ready_label->setText("Ready for commands!");
    }
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

    QHoverSensitiveButton::hoverTime -= 200;

    if (QHoverSensitiveButton::hoverTime <= 0){
        QHoverSensitiveButton::hoverTime = 0;
    }
    QString display = QString::number(double(QHoverSensitiveButton::hoverTime) / 1000.0, 'f', 1) + " secs";
    ui->hover_time_label->setText(display);
}

void MainWindow::hover_time_up(){
    QHoverSensitiveButton::hoverTime += 200;

    QString display = QString::number(double(QHoverSensitiveButton::hoverTime) / 1000.0, 'f', 1) + " secs";
    ui->hover_time_label->setText(display);
}

void MainWindow::changeLabel(){
    if (ui->label->text() == "Hover + Hold"){
        ui->label->setText("Press Button");
        ui->hoverButton->setText("OFF");
    }
    else{
        ui->label->setText("Hover + Hold");
        ui->hoverButton->setText("ON");
    }
}


void MainWindow::auto_move(){
    auto_movement = !auto_movement;
    if (auto_movement){
        ui->autoButton->setText("ON");
    }
    else{
        ui->autoButton->setText("OFF");
    }
}

void MainWindow::move_down(){
    //ui->downButton->setStyleSheet("QPushButton { background-color: red; }\n");
    y_pos += move_speed;
    qDebug() << "Y POS: " + QString::number(y_pos);
    write_to_arduino("1" + QString::number(y_pos) + "X");
    //ui->downButton->setStyleSheet("QPushButton { background-color: rgb(205, 205, 205); }\n");

}
void MainWindow::move_up(){
    //ui->upButton->setStyleSheet("QPushButton { background-color: red; }\n");
    y_pos -= move_speed;
    qDebug() << "Y POS: " + QString::number(y_pos);
    write_to_arduino("1" + QString::number(y_pos) + "X");
}
void MainWindow::move_left(){
    //ui->leftButton->setStyleSheet("QPushButton { background-color: red; }\n");
    x_pos += move_speed;
    qDebug() << "X POS: " + QString::number(x_pos);
    write_to_arduino("0" + QString::number(x_pos) + "X");
}
void MainWindow::move_right(){
    x_pos -= move_speed;
    //ui->rightButton->setStyleSheet("QPushButton { background-color: red; }\n");
    qDebug() << "X POS: " + QString::number(x_pos);
    write_to_arduino("0" + QString::number(x_pos) + "X");
}
void MainWindow::move_forward(){
    if (z_pos < 70){
        z_pos -= move_speed;
        qDebug() << "Z POS: " + QString::number(z_pos);
        write_to_arduino("2" + QString::number(z_pos) + "X");
    }
    else {
        qDebug() << "CAN'T MOVE ANY FARTHER";
    }

}
void MainWindow::move_backward(){
    z_pos += move_speed;
    qDebug() << "Z POS: " + QString::number(z_pos);
    write_to_arduino("2" + QString::number(z_pos) + "X");
}

void MainWindow::move_finished(){
    qDebug() << "Retract";

    if (!popup_open){
        popup_open = true;

        Release popup;
        popup.setVisible(true);
        popup.setModal(true);
        popup.open();
        qDebug() << "opening";
        popup.countdown();
        QHoverSensitiveButton::t.setHMS(-1,-1,-1,-1);
        //qDebug() << "Setting t to " + QHoverSensitiveButton::t.toString();

        QHoverSensitiveButton::hoverButton = "";

        delay(5000);
        qDebug() << "closing";
        popup.done(1);
        popup_open = false;

        x_pos = 93;
        y_pos = 40;
        z_pos = 40;
        write_to_arduino("0" + QString::number(x_pos) + "X");
        write_to_arduino("1" + QString::number(y_pos) + "X");
        write_to_arduino("2" + QString::number(z_pos) + "X");
    }
    else{
       return;
    }

}
