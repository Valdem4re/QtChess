#include "qtchessgraphicsitem.h"

QtChessGraphicsItem::QtChessGraphicsItem(QObject *parent, QPixmap * Apmsel, int x, int y)
{
    this->pm_sel = Apmsel ;
    this->pm_figure = NULL ;
    this->i=x ;
    this->j=y ;
    isover=false ;
    issel=false ;
    hasfigure = false ;
    hasway = false ;

    polygon.append(QPoint(0,0));
    polygon.append(QPoint(FIELDSIZE,0));
    polygon.append(QPoint(FIELDSIZE,FIELDSIZE));
    polygon.append(QPoint(0,FIELDSIZE));
    polygon.append(QPoint(0,0));
}

bool QtChessGraphicsItem::isPiece()
{
    return hasfigure ;
}

void QtChessGraphicsItem::setSelect()
{
    issel = true ;
    update() ;
}

void QtChessGraphicsItem::removeSelect()
{
    if (!issel) return ;
    issel = false ;
    update() ;
}

void QtChessGraphicsItem::setIsOver()
{
    isover = true ;
    update() ;
}

void QtChessGraphicsItem::clearIsOver()
{
    if (!isover) return ;
    isover = false ;
    update() ;
}

void QtChessGraphicsItem::setWay(bool way)
{
    hasway = way ;
}

void QtChessGraphicsItem::setPiece(bool figure, QPixmap * pixmap)
{
    hasfigure = figure ;
    pm_figure = pixmap ;
}

QRectF QtChessGraphicsItem::boundingRect() const
{
    return QRectF(0,0,FIELDSIZE,FIELDSIZE);
}

void QtChessGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // figure
    if (hasfigure) {
    painter->drawPixmap(0,0,*pm_figure);
    if (issel) {
        painter->setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap));
        painter->drawPolyline(polygon) ;
    }
    else
    if (isover) {
        painter->setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
        painter->drawPolyline(polygon) ;
    }
    }

    // way
    if (hasway) {
    painter->drawPixmap(0,0,*pm_sel);
    if (isover) {
        painter->setPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::RoundCap));
        painter->drawPolyline(polygon) ;
    }
    }
}
