#ifndef QTCHESSSERVER_H
#define QTCHESSSERVER_H
#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QMap>

const int CHESS_PORT = 777 ;

class QtChessServer : public QObject
{
    Q_OBJECT
    private:
        // Сервер
        QTcpServer *tcpServer;
        int server_status = 0;
        // Клиенты сервера

        QMap<int,QTcpSocket *> SClients;
        QString savedcmd = "" ;
        QString lastcmd ;
    public:
        explicit QtChessServer(QObject *parent = nullptr);

    signals:

    public slots:
        void newPlayer() ;
        void readFromClient() ;
};

#endif // QTCHESSSERVER_H
