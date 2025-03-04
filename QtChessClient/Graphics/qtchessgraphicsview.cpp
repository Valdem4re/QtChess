#include "qtchessgraphicsview.h"
#include <QPainter>


QPointF calcPos(int i, int j) {
    return QPointF(i*FIELDSIZE+FRAME, j*FIELDSIZE+FRAME) ;
}

void QtChessGraphicsView::clearChess() {
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++) {
            fields[i][j]->setWay(false);
            fields[i][j]->setPiece(false);
        }
}

void QtChessGraphicsView::addPiece(PlayerColor color , PieceType type, int x, int y) {
    QPixmap ** figs = (color== BLACK)?blackPieces:whitePieces ;
    fields[x][y]->setPiece(true,figs[type]);
}

QtChessGraphicsView::QtChessGraphicsView(QWidget *parent, Game *game, PlayerColor playercolor): QGraphicsView(parent)
{
    currPos.setX(-1); currPos.setY(-1);
    this->game = game ;
    this->playercolor = playercolor ;

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setMouseTracking(true);

    selpix = new QPixmap(":/Textures/circle (1).png") ;

    scene = new QGraphicsScene();

    blackPieces = new QPixmap*[6];
    blackPieces[PAWN]=new QPixmap(":/images/Textures/Pieces/x52/Black_Pawn.png");
    blackPieces[BISHOP]=new QPixmap(":/images/Textures/Pieces/x52/Black_Bishop.png");
    blackPieces[KNIGHT]=new QPixmap(":/images/Textures/Pieces/x52/Black_Knight.png");
    blackPieces[ROOK]=new QPixmap(":/images/Textures/Pieces/x52/Black_Rock.png");
    blackPieces[QUEEN]=new QPixmap(":/images/Textures/Pieces/x52/Black_Queen.png");
    blackPieces[KING]=new QPixmap(":/images/Textures/Pieces/x52/Black_King.png");
    whitePieces = new QPixmap*[6] ;
    whitePieces[PAWN]=new QPixmap(":/images/Textures/Pieces/x52/White_Pawn.png");
    whitePieces[BISHOP]=new QPixmap(":/images/Textures/Pieces/x52/White_Bishop.png");
    whitePieces[KNIGHT]=new QPixmap(":/images/Textures/Pieces/x52/White_Knight.png");
    whitePieces[ROOK]=new QPixmap(":/images/Textures/Pieces/x52/White_Rock.png");
    whitePieces[QUEEN]=new QPixmap(":/images/Textures/Pieces/x52/White_Queen.png");
    whitePieces[KING]=new QPixmap(":/images/Textures/Pieces/x52/White_King.png");

    connect(game,SIGNAL(clearChess()),this,SLOT(clearChess())) ;
    connect(game,SIGNAL(addToChess(PlayerColor,PieceType,int,int)),
            this,SLOT(addPiece(PlayerColor,PieceType,int,int))) ;

    this->setScene(scene) ;

    QGraphicsPixmapItem * item = scene->addPixmap(
                QPixmap(playercolor==WHITE?":/images/Textures/Boards/BrownBoard(white_side)_504x504.jpg":
                                           ":/images/Textures/Boards/BrownBoard(black_side)_504x504.jpg")) ;
    item->setPos(0,0) ;

    fields = new QtChessGraphicsItem**[8] ;
    for (int i=0; i<8; i++)
        fields[i] = new QtChessGraphicsItem*[8] ;

    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++) {
            fields[i][j]=new QtChessGraphicsItem(scene,selpix,i,j) ;
            fields[i][j]->setPos(calcPos(i,j)) ;
            scene->addItem(fields[i][j]);
        }

    game->fillBoard();
}

QtChessGraphicsView::~QtChessGraphicsView()
{
    delete scene ;
}

void QtChessGraphicsView::updateSelection(QtChessGraphicsItem *item)
{
    int i = item->i ;
    int j = item->j ;

    for (QGraphicsItem * item: scene->items()) {
        ((QtChessGraphicsItem*)item)->removeSelect();
        ((QtChessGraphicsItem*)item)->setWay(false);
    }

    item->setSelect();
    currPos.setX(i); currPos.setY(j);

    QList<QPoint> steps = game->avaibleMoves(i,j) ;

    for (QPoint sel: steps)
        fields[sel.x()][sel.y()]->setWay(true);
}

void QtChessGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (!game->mystep) return ;

    QPointF scenePos = this->mapToScene(event->pos()) ;

    for (QGraphicsItem * item: scene->items())
        ((QtChessGraphicsItem*)item)->clearIsOver();
    QGraphicsItem * item = scene->itemAt(scenePos,QTransform()) ;
    if (item!=NULL)
        ((QtChessGraphicsItem*)item)->setIsOver();

}

void QtChessGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (!game->mystep) return ;

    if(event->button() != Qt::LeftButton) return ;
    QPointF scenePos = this->mapToScene(event->pos()) ;
    QGraphicsItem * tek = scene->itemAt(scenePos,QTransform()) ;

    if (tek==NULL) return ;

    if (tek->pos().x()<1) return ;

    QtChessGraphicsItem * fig = (QtChessGraphicsItem*)tek ;

    // Защита от клика по самой себе
    if ((fig->x()==currPos.x())&&(fig->y()==currPos.y())) return ;

    GameAction act = NONE ;

    if (fig->isPiece()) {
         if (game->isPlayerFigure(fig->i,fig->j)) {
             act = SELECT ;
             if (currPos.x()>=0)
                if (game->isCastling(currPos.x(),currPos.y(),fig->i,fig->j))
                    act = CASTLING ;
         }
         else
             act = MOVE ;
    }
    else
        act = MOVE ;

    if (act == SELECT) {
        for (QGraphicsItem * item: scene->items())
             ((QtChessGraphicsItem*)item)->removeSelect();

        fig->setSelect();
        updateSelection(fig) ;
        scene->update(0,0,1000,1000);
    }
    if (act == MOVE)
        if (game->makeMove(currPos.x(),currPos.y(),fig->i,fig->j,true)) {
            game->fillBoard();
            scene->update(0,0,1000,1000);
            currPos.setX(-1); currPos.setY(-1);
        }
    if (act == CASTLING)
        if (game->makeCastling(currPos.x(),currPos.y(),fig->i,fig->j)) {
            game->fillBoard();
            scene->update(0,0,1000,1000);
            currPos.setX(-1); currPos.setY(-1);
        }

}
void QtChessGraphicsView::keyPressEvent(QKeyEvent *e)
{
}

