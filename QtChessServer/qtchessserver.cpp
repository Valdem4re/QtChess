#include "qtchessserver.h"
#include <QDebug>

QtChessServer::QtChessServer(QObject *parent): QObject(parent)
{
    tcpServer = new QTcpServer(this);
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newPlayer()));
        if (!tcpServer->listen(QHostAddress::Any, CHESS_PORT) && server_status==0) {
            qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        } else {
            qDebug() << "Server started at port"<<CHESS_PORT ;
            server_status=1;
        }
}

void QtChessServer::newPlayer()
{
    if(server_status==1){
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        SClients[idusersocs]=clientSocket;
        connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(readFromClient()));
        qDebug() << "Client connected" ;
    }
}

void QtChessServer::readFromClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    QByteArray data = clientSocket->readAll();
    QString cmd = QString::fromLatin1(data) ;
    if (cmd!=lastcmd) qDebug() << "Get command: "<<cmd ;
    lastcmd = cmd ;
    if (cmd.startsWith("START")) {
        if (SClients.count()==1) clientSocket->write("WHITE") ; else
        if (SClients.count()==2) clientSocket->write("BLACK") ; else
        clientSocket->write("CLOSED") ;
    }
    if (cmd.startsWith("STEP")) {
        clientSocket->write("OK") ;
        savedcmd = cmd ;
    }
    if (cmd.startsWith("ROCK")) {
        clientSocket->write("OK") ;
        savedcmd = cmd ;
    }
    if (cmd.startsWith("WAIT")) {
        if (savedcmd.length()>0) {
            QByteArray res(savedcmd.toLocal8Bit()) ;
            clientSocket->write(res) ;
            savedcmd="" ;
        }
        else {
            clientSocket->write("NOP") ;
        }
    }
}
