#include "guimain.h"
#include "ui_guimain.h"

GUIMain::GUIMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GUIMain)
{
    ui->setupUi(this);

    _eegSocket = new QTcpSocket(this);
    _keySocket = new QTcpSocket(this);
    _soundSocket = new QTcpSocket(this);
    _imageSocket = new QTcpSocket(this);

    //QMetaObject::connectSlotsByName(this);
    connect(_eegSocket, SIGNAL(readyRead()), this, SLOT(eeg_ready()));
    connect(_keySocket, SIGNAL(readyRead()), this, SLOT(key_ready()));
    connect(_soundSocket, SIGNAL(readyRead()), this, SLOT(sound_ready()));
    connect(_imageSocket, SIGNAL(readyRead()), this, SLOT(image_ready()));

    connect(_eegSocket, SIGNAL(connected()), this, SLOT(eeg_connected()));
    connect(_keySocket, SIGNAL(connected()), this, SLOT(key_connected()));
    connect(_soundSocket, SIGNAL(connected()), this, SLOT(sound_connected()));
    connect(_imageSocket, SIGNAL(connected()), this, SLOT(image_connected()));

    connect(_eegSocket, SIGNAL(disconnected()), this, SLOT(eeg_disconnected()));
    connect(_keySocket, SIGNAL(disconnected()), this, SLOT(key_disconnected()));
    connect(_soundSocket, SIGNAL(disconnected()), this, SLOT(sound_disconnected()));
    connect(_imageSocket, SIGNAL(disconnected()), this, SLOT(image_disconnected()));

    connect(_eegSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(eeg_error(QAbstractSocket::SocketError)));
    connect(_keySocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(key_error(QAbstractSocket::SocketError)));
    connect(_soundSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sound_error(QAbstractSocket::SocketError)));
    connect(_imageSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(image_error(QAbstractSocket::SocketError)));
}

GUIMain::~GUIMain()
{
    delete ui;
}

void GUIMain::on_eegconn_clicked(){

    QPushButton* button = findChild<QPushButton*>("eegconn");

    if (button->text().compare("EEG Connect") == 0){

        button->setEnabled(false);

        _eegSocket->connectToHost("localhost", EEG_PORT);
    }
    else {

        //button->setText("EEG Connect");

        _eegSocket->abort();
    }

}

void GUIMain::on_eegrecord_clicked(){

    if (!_eegSocket->isOpen()) {
        QMessageBox::information(this, tr("EEG Host"),
                                 tr("EEG socket not open"));
        return;
    }

    QPushButton* button = findChild<QPushButton*>("eegrecord");

    QTextStream* message = new QTextStream(_eegSocket);
    if (button->text().compare("EEGRecord Start") == 0){

        button->setText("EEGRecord Stop");

        *message << "<TRIGGER>1</TRIGGER>";

    }
    else {

        button->setText("EEGRecord Start");

        *message << "<TRIGGER>2</TRIGGER>";
    }

    message->flush();
    delete(message);
}

void GUIMain::on_keyconn_clicked(){

    QPushButton* button = findChild<QPushButton*>("keyconn");

    if (button->text().compare("KeyLogger Connect") == 0){

        button->setEnabled(false);

        _keySocket->connectToHost("localhost", KEY_PORT);
    }
    else {

        _keySocket->abort();
    }

}

void GUIMain::on_keyrecord_clicked(){

    if (!_keySocket->isOpen()) {
        QMessageBox::information(this, tr("Keylogger Host"),
                                 tr("KeyLogger socket not open"));
        return;
    }

    QPushButton* button = findChild<QPushButton*>("keyrecord");

    QTextStream* message = new QTextStream(_keySocket);

    if (button->text().compare("KeyRecord Start") == 0){

        button->setText("KeyRecord Stop");

        *message << "2";
    }

    else {

        button->setText("KeyRecord Start");

        *message << "0";
    }

    message->flush();
    delete(message);
}


void GUIMain::on_soundconn_clicked(){
    QPushButton* button = findChild<QPushButton*>("soundconn");

    if (button->text().compare("Sound Connect") == 0){

        button->setEnabled(false);

        _soundSocket->connectToHost("localhost", SOUND_PORT);
    }
    else {

        _soundSocket->abort();
    }

}


void GUIMain::on_sound_clicked(){
//    if (!_soundSocket->isOpen()) {
//        QMessageBox::information(this, tr("Sound Host"),
//                                 tr("Sound socket not open"));
//        return;
//    }

    QPushButton* button = findChild<QPushButton*>("sound");

    QTextStream* message = new QTextStream(_soundSocket);

    if (button->text().compare("Sound Distractor Start") == 0){

        button->setText("Sound Distractor Stop");

        *message << "2";
    }

    else {

        button->setText("Sound Distractor Start");

        *message << "0";
    }

    message->flush();
    delete(message);
}


void GUIMain::on_imageconn_clicked(){
    QPushButton* button = findChild<QPushButton*>("imageconn");

    if (button->text().compare("Image Connect") == 0){

        button->setEnabled(false);

        _imageSocket->connectToHost(VISUAL_HOST, IMAGE_PORT);
    }
    else {

        _imageSocket->abort();
    }
}


void GUIMain::on_image_clicked(){
    if (!_imageSocket->isOpen()) {
        QMessageBox::information(this, tr("Image Host"),
                                 tr("Image socket not open"));
        return;
    }

    QPushButton* button = findChild<QPushButton*>("image");

    QTextStream* message = new QTextStream(_imageSocket);

    if (button->text().compare("Image Distractor Start") == 0){

        button->setText("Image Distractor Stop");

        *message << "2";
    }

    else {

        button->setText("Image Distractor Start");

        *message << "0";
    }

    message->flush();
    delete(message);
}


void GUIMain::on_allrecord_clicked(){

    QPushButton* button = findChild<QPushButton*>("allrecord");

    if (button->text().compare("All Start") == 0){

        button->setText("All Stop");
    }
    else button->setText("All Start");

    on_eegrecord_clicked();

    on_keyrecord_clicked();

    on_sound_clicked();

    on_image_clicked();
}


void GUIMain::eeg_ready(){
    cout << "there is some data that should not be around on EEG socket";
}


void GUIMain::key_ready(){
    cout << "there is some data that should not be around on KEY socket";
}


void GUIMain::sound_ready(){
    cout << "there is some data that should not be around on SOUND socket";
}


void GUIMain::image_ready(){
    cout << "there is some data that should not be around on IMAGE socket";
}


void GUIMain::eeg_connected(){

    QPushButton* startButton = findChild<QPushButton*>("eegrecord");
    startButton->setEnabled(true);

    QPushButton* button = findChild<QPushButton*>("eegconn");
    button->setText("EEG Disconnect");
    button->setEnabled(true);

    if (_keySocket->isOpen() && _soundSocket->isOpen() && _imageSocket->isOpen()) {
        QPushButton* p = findChild<QPushButton*>("allrecord");
        p->setEnabled(true);
    }
}


void GUIMain::eeg_disconnected(){
    QPushButton* startButton = findChild<QPushButton*>("eegrecord");
    startButton->setEnabled(false);
    startButton->setText("EEGRecord Start");

    QPushButton* button = findChild<QPushButton*>("eegconn");
    button->setText("EEG Connect");
    button->setEnabled(true);
    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::key_connected(){

    QPushButton* startButton = findChild<QPushButton*>("keyrecord");
    startButton->setEnabled(true);

    QPushButton* button = findChild<QPushButton*>("keyconn");
    button->setText("KeyLogger Disconnect");
    button->setEnabled(true);

    if (_eegSocket->isOpen() && _soundSocket->isOpen() && _imageSocket->isOpen()) {
        QPushButton* p = findChild<QPushButton*>("allrecord");
        p->setEnabled(true);
    }
}


void GUIMain::key_disconnected(){
    QPushButton* startButton = findChild<QPushButton*>("keyrecord");
    startButton->setEnabled(false);

    QPushButton* button = findChild<QPushButton*>("keyconn");
    button->setText("KeyLogger Connect");
    button->setEnabled(true);
    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::sound_connected(){
    QPushButton* soundButton = findChild<QPushButton*>("sound");
    soundButton->setEnabled(true);

    QPushButton* connButton = findChild<QPushButton*>("soundconn");
    connButton->setText("Sound Disconnect");
    connButton->setEnabled(true);

    if (_eegSocket->isOpen() && _keySocket->isOpen() && _imageSocket->isOpen()) {
        QPushButton* p = findChild<QPushButton*>("allrecord");
        p->setEnabled(true);
    }
}


void GUIMain::sound_disconnected(){
    QPushButton* soundButton = findChild<QPushButton*>("sound");
    soundButton->setEnabled(false);
    soundButton->setText("Sound Distractor Start");

    QPushButton* connButton = findChild<QPushButton*>("soundconn");
    connButton->setText("Sound Connect");
    connButton->setEnabled(true);

    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::image_connected(){
    QPushButton* imageButton = findChild<QPushButton*>("image");
    imageButton->setEnabled(true);

    QPushButton* connButton = findChild<QPushButton*>("imageconn");
    connButton->setText("Image Disconnect");
    connButton->setEnabled(true);

    if (_eegSocket->isOpen() && _keySocket->isOpen() && _soundSocket->isOpen()) {
        QPushButton* p = findChild<QPushButton*>("allrecord");
        p->setEnabled(true);
    }
}


void GUIMain::image_disconnected(){
    QPushButton* imageButton = findChild<QPushButton*>("image");
    imageButton->setEnabled(false);
    imageButton->setText("Image Distractor Start");

    QPushButton* connButton = findChild<QPushButton*>("imageconn");
    connButton->setText("Image Connect");
    connButton->setEnabled(true);

    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::eeg_error(QAbstractSocket::SocketError socketError){

    switch (socketError) {

        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("EEG Host"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("EEG Host"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("EEG Host"),
                                     tr("The following error occurred: %1.")
                                     .arg(_eegSocket->errorString()));
        }
    QPushButton* startButton = findChild<QPushButton*>("eegrecord");
    startButton->setEnabled(false);

    QPushButton* button = findChild<QPushButton*>("eegconn");
    button->setText("EEG Connect");
    button->setEnabled(true);

    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::key_error(QAbstractSocket::SocketError socketError){
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("KeyLogger Host"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("KeyLogger Host"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("KeyLogger Host"),
                                     tr("The following error occurred: %1.")
                                     .arg(_keySocket->errorString()));
        }
    QPushButton* startButton = findChild<QPushButton*>("keyrecord");
    startButton->setEnabled(false);

    QPushButton* button = findChild<QPushButton*>("keyconn");
    button->setText("KeyLogger Connect");
    button->setEnabled(true);

    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::sound_error(QAbstractSocket::SocketError socketError){
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("Sound Host"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Sound Host"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("Sound Host"),
                                     tr("The following error occurred: %1.")
                                     .arg(_soundSocket->errorString()));
        }
    QPushButton* startButton = findChild<QPushButton*>("sound");
    startButton->setEnabled(false);

    QPushButton* button = findChild<QPushButton*>("soundconn");
    button->setText("Sound Connect");
    button->setEnabled(true);

    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}


void GUIMain::image_error(QAbstractSocket::SocketError socketError){
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("Image Host"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Image Host"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("Image Host"),
                                     tr("The following error occurred: %1.")
                                     .arg(_imageSocket->errorString()));
        }
    QPushButton* startButton = findChild<QPushButton*>("image");
    startButton->setEnabled(false);

    QPushButton* button = findChild<QPushButton*>("imageconn");
    button->setText("Image Connect");
    button->setEnabled(true);

    QPushButton* p = findChild<QPushButton*>("allrecord");
    p->setEnabled(false);
}
