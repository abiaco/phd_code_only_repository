#ifndef GUIMAIN_H
#define GUIMAIN_H

#include <QMainWindow>
#include <QtNetwork>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>
#include "constants.h"

using namespace std;

namespace Ui {
class GUIMain;
}

class GUIMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUIMain(QWidget *parent = 0);
    ~GUIMain();

private:
    Ui::GUIMain *ui;
    QTcpSocket *_eegSocket, *_keySocket, *_soundSocket, *_imageSocket;
private slots:
    void eeg_ready();
    void key_ready();
    void sound_ready();
    void image_ready();

    void eeg_connected();
    void key_connected();
    void sound_connected();
    void image_connected();

    void eeg_disconnected();
    void key_disconnected();
    void sound_disconnected();
    void image_disconnected();

    void eeg_error(QAbstractSocket::SocketError socketError);
    void key_error(QAbstractSocket::SocketError socketError);
    void sound_error(QAbstractSocket::SocketError socketError);
    void image_error(QAbstractSocket::SocketError socketError);

    void on_eegconn_clicked();
    void on_eegrecord_clicked();

    void on_keyconn_clicked();
    void on_keyrecord_clicked();

    void on_soundconn_clicked();
    void on_sound_clicked();

    void on_imageconn_clicked();
    void on_image_clicked();

    void on_allrecord_clicked();
};

#endif // GUIMAIN_H
