#ifndef QTCHESSGRAPHICSITEM_H
#define QTCHESSGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

// Базовый элемент для вывода на доске
class QtChessGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit QtChessGraphicsItem(QObject *parent, QPixmap * Apmsel, int x, int y) ;

    int i; int j;
    // Управление выделением
    void setIsOver() ;

    void clearIsOver() ;

    void setSelect() ;

    void removeSelect() ;


    bool isPiece() ;

    void setWay(bool way) ;

    void setPiece(bool figure, QPixmap * pixmap = NULL) ;

protected:
    // Текстуры
    QPixmap * pm_figure ;
    QPixmap * pm_sel ;
    // Флаги
    bool hasfigure ;
    bool hasway ;

    bool isover ;
    bool issel ;
    // Размер
    QPolygon polygon ;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) ;
};

// Размер ячейки доски
const int FIELDSIZE = 52 ;

#endif // QTCHESSGRAPHICSITEM_H
