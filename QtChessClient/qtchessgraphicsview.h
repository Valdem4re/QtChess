#ifndef QTCHESSGRAPHICSVIEW_H
#define QTCHESSGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QKeyEvent>
#include "game.h"
#include "qtchessgraphicsitem.h"

// Размер рамки доски
const int FRAME = 44 ;

// Класс просмотра доски
class QtChessGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QtChessGraphicsView(QWidget *parent, Game * game, PlayerColor playercolor);
    ~QtChessGraphicsView();


    QGraphicsScene * scene;

private:

    QtChessGraphicsItem *** fields;

    Game * game ;

    // Текстуры фигур
    QPixmap ** blackPieces;
    QPixmap ** whitePieces;

    QPixmap * selpix;

    QPoint currPos;

    PlayerColor playercolor ;

    // Обновление доступных ходов фигуры
    void updateSelection(QtChessGraphicsItem * item) ;

    enum GameAction {SELECT, MOVE, CASTLING, NONE } ;
protected:

    // Обработка движений и нажатия мыши
    virtual void mouseMoveEvent ( QMouseEvent * event );

    virtual void mousePressEvent ( QMouseEvent * event );

    virtual void keyPressEvent(QKeyEvent* e);

public slots:
    // Слоты для получения данных от игры
    void clearChess();

    void addPiece(PlayerColor color,PieceType type, int x, int y);
};



#endif // QTCHESSGRAPHICSVIEW_H
