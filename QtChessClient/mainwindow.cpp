#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ontimer()));
}

void MainWindow::ontimer()
{
    QByteArray waits("WAIT") ;
    client->write(waits) ;
    client->waitForReadyRead(1000) ;

    QByteArray data = client->readAll();

    QString cmd = QString::fromLatin1(data) ;
    if (cmd.startsWith("STEP")) {
        int i1 = cmd[4].digitValue();
        int j1 = cmd[5].digitValue();
        int i2 = cmd[6].digitValue();
        int j2 = cmd[7].digitValue();

        game->makeMove(7-i1,7-j1,7-i2,7-j2,false);

        game->fillBoard();

        view->scene->invalidate(0,0,1000,1000);

        setMsg(MSG_MYSTEP) ;
        game->mystep=true ;
        timer->stop() ;
    }
    if (cmd.startsWith("ROCK")) {
        int i1 = cmd[4].digitValue();
        int i2 = cmd[5].digitValue();
        int xk = cmd[6].digitValue();
        int xt = cmd[7].digitValue();

        game->applyCastling(7-i1,7-i2,7-xk,7-xt,false);
        game->fillBoard();

        view->scene->invalidate(0,0,1000,1000);

        setMsg(MSG_MYSTEP) ;
        game->mystep=true ;
        timer->stop() ;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ButClientRun_clicked()
{
    ui->ButClientRun->hide();
    ui->label->hide();
    ui->edHost->hide();

    client = new QTcpSocket(this) ;

    client->connectToHost(ui->edHost->text(), CHESS_PORT);
    if (client->waitForConnected()) {
        if(client->state() == QAbstractSocket::ConnectedState)
            {
                client->write("START") ;
                client->waitForReadyRead(1000) ;
                QByteArray arr = client->readAll() ;
                QString res = QString::fromLocal8Bit(arr) ;
                if (res=="WHITE") {
                    newGame(WHITE) ;
                    setMsg(MSG_MYSTEP) ;
                }
                else
                if (res=="BLACK") {
                    newGame(BLACK) ;
                    setMsg(MSG_WAITSTEP) ;
                    timer->start(1000) ;
                }
                else {
                    setMsg("Игра уже заполнена");
                    client->close();
                    return ;
                }
            }
        else
            qDebug()<<"Ошибка подключения 1";
    }
    else
        qDebug()<<"Ошибка подключения 2";
}

void MainWindow::setMsg(QString msg)
{
    ui->labStepInfo->setText(msg);
}

void MainWindow::newGame(PlayerColor color)
{    
    game = new Game(color) ;    
    connect(game,SIGNAL(sendStepOverSocket(int,int,int,int)),
        this,SLOT(sendStep(int,int,int,int))) ;

    connect(game,SIGNAL(sendRockOverSocket(int,int,int,int)),
        this,SLOT(sendRock(int,int,int,int)));

    view = new QtChessGraphicsView(ui->centralWidget,game,game->getPlayerColor());

    view->setGeometry(30,60,2*FRAME+FIELDSIZE*8+4,2*FRAME+FIELDSIZE*8+4);

    view->show();
}

void MainWindow::sendStep(int i1, int j1, int i2, int j2)
{
    QByteArray cmd(QString("STEP%1%2%3%4").arg(i1).arg(j1).arg(i2).arg(j2).toLocal8Bit());
    client->write(cmd) ;
    client->waitForReadyRead(1000);

    QByteArray arr = client->readAll();

    setMsg(MSG_WAITSTEP);
    game->mystep=false;
    timer->start(1000);
}

void MainWindow::sendRock(int x1, int i2, int xk, int xt)
{
    QByteArray cmd(QString("ROCK%1%2%3%4").arg(x1).arg(i2).arg(xk).arg(xt).toLocal8Bit()) ;
    client->write(cmd) ;
    client->waitForReadyRead(1000) ;
    QByteArray arr = client->readAll() ;
    setMsg(MSG_WAITSTEP) ;
    game->mystep=false ;
    timer->start(1000) ;
}
