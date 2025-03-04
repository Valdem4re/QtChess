#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtchessgraphicsview.h"
#include "game.h"
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <QTime>

const int CHESS_PORT = 777;

namespace Ui {
class MainWindow;
}

class QTcpServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_ButClientRun_clicked();

    void ontimer() ;

private:

    Ui::MainWindow *ui;

    QtChessGraphicsView * view;

    Game * game ;

    QTcpSocket * client;

    // Установка сообщения
    void setMsg(QString msg);
    void newGame(PlayerColor color);

    // Таймер для запроса хода сервера
    QTimer * timer ;
public slots:
    // Отправка хода на сервер
    void sendStep(int i1, int j1, int i2, int j2);

    void sendRock(int x1, int x2, int xKing, int xRook);
};

const QString MSG_MYSTEP = "Your turn";
const QString MSG_WAITSTEP = "Waiting for the opponent's move";

#endif // MAINWINDOW_H
