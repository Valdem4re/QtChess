#include "game.h"
#include <QDebug>

void Game::addPiece(PlayerColor color, PieceType type, int x, int y)
{
    Piece * fig = new Piece() ;
    fig->type=type ;
    fig->color=color ;
    fields[x][y]=fig ;
}

Game::Game(PlayerColor playercolor) : QObject(NULL)
{
    this->playercolor=playercolor ;
    this->mystep=(playercolor == WHITE) ;

    enemycolor = (playercolor == WHITE )? BLACK :WHITE ;

    fields = new Piece**[SZ] ;
    for (int i=0; i<SZ; i++) {
        fields[i]=new Piece*[SZ] ;
        for (int j=0; j<SZ; j++)
            fields[i][j]=NULL ;
    }

    //ставим пешки
    for(int i =0; i < 8; ++i){
        addPiece(playercolor, PAWN, i, 6);
        addPiece(enemycolor, PAWN, i, 1);
    }
    //мои фигуры
    addPiece(playercolor,ROOK, 0, 7);
    addPiece(playercolor, KNIGHT, 1, 7);
    addPiece(playercolor, BISHOP, 2, 7);
    addPiece(playercolor, BISHOP, 5, 7);
    addPiece(playercolor, KNIGHT, 6, 7);
    addPiece(playercolor, ROOK, 7, 7);

    //фигуры соперника
    addPiece(enemycolor,ROOK, 0, 0);
    addPiece(enemycolor, KNIGHT, 1, 0);
    addPiece(enemycolor, BISHOP, 2, 0);
    addPiece(enemycolor, BISHOP, 5, 0);
    addPiece(enemycolor, KNIGHT, 6, 0);
    addPiece(enemycolor, ROOK, 7, 0);

    if(playercolor == WHITE){
        addPiece(playercolor,QUEEN, 3, 7);
        addPiece(playercolor, KING, 4, 7);
        addPiece(enemycolor, QUEEN, 3,0);
        addPiece(enemycolor, KING, 4,0);
    }
    else{
        addPiece(playercolor,QUEEN, 4, 7);
        addPiece(playercolor, KING, 3, 7);
        addPiece(enemycolor, QUEEN, 4,0);
        addPiece(enemycolor, KING, 3,0);
    }
}

bool Game::isEmpty(int x, int y)
{
    return fields[x][y]==NULL ;
}

bool Game::isEmpty(QPoint p)
{
    return fields[p.x()][p.y()]==NULL ;
}

void Game::fillBoard()
{
    emit clearChess();
    for (int i=0; i<SZ; i++)
        for (int j=0; j<SZ; j++)
            if (!isEmpty(i,j))
                emit addToChess(fields[i][j]->color,fields[i][j]->type,i,j);
}

bool isIn(QPoint p) {
    return (p.x()>=0)&&(p.y()>=0)&&(p.x()<SZ)&&(p.y()<SZ) ;
}

bool isIn(int i, int j) {
    return isIn(QPoint(i,j)) ;
}

bool Game::buildMyAttack(QList<QPoint> & list, int i, int j) {
    QPoint p(i,j) ;
    if (isEmpty(p)) {
        list.append(p);
        return false ;
    }
    if (isEnemyFigure(p)) {
        list.append(p);
        return true ;
    }
    else
        return true ;
}

bool Game::buildEnemyAttack(QList<QPoint> & lst, int x, int y) {
    QPoint p(x,y) ;
    if (!isIn(p)) return true ;

    lst.append(p);
    return !isEmpty(p) ;
}

void Game::buildMyBishopAttack(QList<QPoint> & lst, int x, int y) {
    for (int ki=x+1, kj=y+1; isIn(ki,kj); ki++, kj++)
        if (buildMyAttack(lst,ki,kj)) break ;
    for (int ki=x+1, kj=y-1; isIn(ki,kj); ki++, kj--)
        if (buildMyAttack(lst,ki,kj)) break ;
    for (int ki=x-1, kj=y+1; isIn(ki,kj); ki--, kj++)
        if (buildMyAttack(lst,ki,kj)) break ;
    for (int ki=x-1, kj=y-1; isIn(ki,kj); ki--, kj--)
        if (buildMyAttack(lst,ki,kj)) break ;
}

void Game::buildMyRookAttack(QList<QPoint> & lst, int x, int y) {
for (int ki=x+1; isIn(ki,y); ki++)
    if (buildMyAttack(lst,ki,y)) break ;
for (int ki=x-1; isIn(ki,y); ki--)
    if (buildMyAttack(lst,ki,y)) break ;
for (int kj=y+1; isIn(x,kj); kj++)
    if (buildMyAttack(lst,x,kj)) break ;
for (int kj=y-1; isIn(x,kj); kj--)
    if (buildMyAttack(lst,x,kj)) break ;
}

void Game::buildEnemyBishopAttack(QList<QPoint> & lst, int x, int y) {
    for (int ki=x+1, kj=y+1; isIn(ki,kj); ki++, kj++)
        if (buildEnemyAttack(lst,ki,kj)) break ;
    for (int ki=x+1, kj=y-1; isIn(ki,kj); ki++, kj--)
        if (buildEnemyAttack(lst,ki,kj)) break ;
    for (int ki=x-1, kj=y+1; isIn(ki,kj); ki--, kj++)
        if (buildEnemyAttack(lst,ki,kj)) break ;
    for (int ki=x-1, kj=y-1; isIn(ki,kj); ki--, kj--)
        if (buildEnemyAttack(lst,ki,kj)) break ;
}

void Game::buildEnemyRookAttack(QList<QPoint> & lst, int x, int y) {
for (int ki=x+1; isIn(ki,y); ki++)
    if (buildEnemyAttack(lst,ki,y)) break ;
for (int ki=x-1; isIn(ki,y); ki--)
    if (buildEnemyAttack(lst,ki,y)) break ;
for (int kj=y+1; isIn(x,kj); kj++)
    if (buildEnemyAttack(lst,x,kj)) break ;
for (int kj=y-1; isIn(x,kj); kj--)
    if (buildEnemyAttack(lst,x,kj)) break ;
}

QList<QPoint> Game::buildEnemyAttacked()
{
    QList<QPoint> r ;
    for (int i=0; i<SZ; i++)
        for (int j=0; j<SZ; j++)
            if (isEnemyFigure(i,j)) {
                int code = fields[i][j]->type ;
                switch(code) {
                case PAWN: {
                    QPoint p1(i-1,j-1) ;  r.append(p1);
                    QPoint p2(i+1,j-1) ;  r.append(p2);
                    break ;
                }
                case KNIGHT: {
                    r.append(QPoint(i-1,j-2)) ; r.append(QPoint(i-2,j-1)) ;
                    r.append(QPoint(i+1,j-2)) ; r.append(QPoint(i+2,j-1)) ;
                    r.append(QPoint(i-1,j+2)) ; r.append(QPoint(i-2,j+1)) ;
                    r.append(QPoint(i+1,j+2)) ; r.append(QPoint(i+2,j+1)) ;
                    break ;
                }
                case KING: {
                    r.append(QPoint(i-1,j)) ; r.append(QPoint(i+1,j)) ;
                    r.append(QPoint(i-1,j-1)) ; r.append(QPoint(i,j-1)) ; r.append(QPoint(i+1,j-1)) ;
                    r.append(QPoint(i-1,j+1)) ; r.append(QPoint(i,j+1)) ; r.append(QPoint(i+1,j+1)) ;
                    break ;
                }
                case ROOK: {
                    buildEnemyRookAttack(r,i,j) ;
                    break ;
                }
                case BISHOP: {
                    buildEnemyBishopAttack(r,i,j) ;
                    break ;
                }
                case QUEEN: {
                    buildEnemyRookAttack(r,i,j) ;
                    buildEnemyBishopAttack(r,i,j) ;
                    break ;
                }
                default:
                    break ;
                }
            }

    return r;
}

QList<QPoint> Game::avaibleMoves(int x, int y)
{
    PieceType type = fields[x][y]->type ;

    QList<QPoint> r ;

    switch(type) {
    case PAWN: {
        QPoint p1(x-1,y-1) ;
        if (isEnemyFigure(p1))
            r.append(p1);
        else {
        // Добавляем взятие на проходе
        if (y==3) {
            if (isEmpty(p1)&&(isEnemyPesh(x-1,y))) r.append(p1);
        }
        }

        QPoint p2(x+1,y-1) ;
        if (isEnemyFigure(p2))
            r.append(p2);
        else {
        // Добавляем взятие на проходе
        if (y==3) {
            if (isEmpty(p2)&&(isEnemyPesh(x+1,y))) r.append(p2);
        }
        }

        QPoint p3(x,y-1) ;
        if (isEmpty(p3)) r.append(p3);
        if (y==6) {
            QPoint p4(x,y-2) ;
            if (isEmpty(p4)) r.append(p4);
        }
        break ;
    }
    case ROOK: {
        buildMyRookAttack(r,x,y) ;
        break ;
    }
    case BISHOP: {
        buildMyBishopAttack(r,x,y) ;
        break ;
    }
    case QUEEN: {
        buildMyRookAttack(r,x,y) ;
        buildMyBishopAttack(r,x,y) ;
        break ;
    }
    case KING: {
        QList<QPoint> attacked = buildEnemyAttacked() ;
        QList<QPoint> steps ;        
        steps.append(QPoint(x-1,y)) ; steps.append(QPoint(x+1,y)) ;
        steps.append(QPoint(x-1,y-1)) ; steps.append(QPoint(x,y-1)) ; steps.append(QPoint(x+1,y-1)) ;
        steps.append(QPoint(x-1,y+1)) ; steps.append(QPoint(x,y+1)) ; steps.append(QPoint(x+1,y+1)) ;
        for (QPoint p: steps)
            if (isIn(p))
                if (!attacked.contains(p))
                    if (isEmpty(p)||isEnemyFigure(p)) r.append(p);
        break ;
    }
    case KNIGHT: {
        QList<QPoint> steps ;
        steps.append(QPoint(x-1,y-2)) ; steps.append(QPoint(x-2,y-1)) ;
        steps.append(QPoint(x+1,y-2)) ; steps.append(QPoint(x+2,y-1)) ;
        steps.append(QPoint(x-1,y+2)) ; steps.append(QPoint(x-2,y+1)) ;
        steps.append(QPoint(x+1,y+2)) ; steps.append(QPoint(x+2,y+1)) ;
        for (QPoint p: steps)
            if (isIn(p))
                if (isEmpty(p)||isEnemyFigure(p)) r.append(p);
        break ;
    }
    default:
        break ;
    }

    return r ;
}

bool Game::isCastling(int x1, int y1, int x2, int y2)
{
    // Здесь проверка корректности рокировки
    if (fields[x1][y1]==NULL) return false ;
    if (fields[x2][y2]==NULL) return false ;
    if (fields[x1][y1]->color!=playercolor) return false ;
    if (fields[x2][y2]->color!=playercolor) return false ;

    if ((y1!=7)||(y2!=7)) return false ;

    int kingi=(playercolor == WHITE)?4:3 ;

    if (x1>x2) {
        int tmp=x1 ; x1=x2 ; x2=tmp ;
    }

    // Ладья слева
    if ((x1==0)&&(fields[x1][7]->type== ROOK)) {
        if (x2==kingi)
            if (fields[x2][7]->type==KING) {
                int xk=kingi-2 ;
                int xt=kingi-1 ;
                return (fields[xk][7]==NULL)&&(fields[xt][7]==NULL) ;
            }
    }
    else
    // Ладья справа
    if ((x2==7)&&(fields[x2][7]->type==ROOK)) {
        if (x1==kingi)
            if (fields[x1][7]->type==KING) {
                int xk=kingi+2 ;
                int xt=kingi+1 ;
                return (fields[xk][7]==NULL)&&(fields[xt][7]==NULL) ;
            }
    }

    return false ;
}

bool Game::makeMove(int x1, int y1, int x2, int y2, bool playerstep)
{
    if (playerstep) {
        auto steps = avaibleMoves(x1,y1) ;
        if (!steps.contains(QPoint(x2,y2))) return false ;
    }

    // Добавляем взятие на проходе
    if (fields[x1][y1]->type==PAWN)
        if ( (playerstep&&((y1==3)&&(y2==2))) ||
           ((!playerstep)&&((y1==4)&&(y2==5))) )
            if (x1!=x2)
                if ( (playerstep && isEnemyPesh(x2,y1)) ||
                   ((!playerstep) && isPlayerPesh(x2,y1)) )
                    fields[x2][y1]=NULL ;

    fields[x2][y2]=fields[x1][y1] ;
    fields[x1][y1]=NULL ;

    if (playerstep) emit sendStepOverSocket(x1,y1,x2,y2);
    return true ;
}

bool Game::makeCastling(int x1, int y1, int x2, int y2)
{
    // Здесь рокировка
    int xk = 0 ;
    int xt = 0 ;

    int kingi=(playercolor==WHITE)?4:3 ;

    if (x1>x2) {
        int tmp=x1 ; x1=x2 ; x2=tmp ;
    }

    // Ладья слева
    if ((x1==0)&&(fields[x1][7]->type==ROOK)) {
        if (x2==kingi)
            if (fields[x2][7]->type==KING) {
                xk=kingi-2 ;
                xt=kingi-1 ;
            }
    }
    else
    // Ладья справа
    if ((x2==7)&&(fields[x2][7]->type==ROOK)) {
        if (x1==kingi)
            if (fields[x1][7]->type==KING) {
                xk=kingi+2 ;
                xt=kingi+1 ;
            }
    }

    applyCastling(x1,x2,xk,xt,true) ;

    emit sendRockOverSocket(x1,x2,xk,xt);
    return true ;
}

void Game::applyCastling(int x1, int x2, int xKing, int xRook, bool isplayer)
{
    int j=isplayer?7:0 ;
    PlayerColor color = isplayer ? playercolor : enemycolor ;
    fields[x1][j]=NULL ;
    fields[x2][j]=NULL ;
    addPiece(color,KING,xKing,j);
    addPiece(color,ROOK,xRook,j);
}

PlayerColor Game::getPlayerColor()
{
    return playercolor ;
}

bool Game::isPlayerFigure(int x, int y)
{
    if (!isIn(x,y)) return false ;
    if (fields[x][y]==NULL) return false ;
    return fields[x][y]->color==playercolor ;
}

bool Game::isEnemyFigure(int i, int j)
{
    if (!isIn(i,j)) return false ;
    if (fields[i][j]==NULL) return false ;
    return fields[i][j]->color!=playercolor ;
}

bool Game::isEnemyPesh(int x, int y)
{
    if (!isIn(x,y)) return false ;
    if (fields[x][y]==NULL) return false ;
    return (fields[x][y]->color!=playercolor)&&(fields[x][y]->type==PAWN) ;
}

bool Game::isPlayerPesh(int x, int y)
{
    if (!isIn(x,y)) return false ;
    if (fields[x][y]==NULL) return false ;
    return (fields[x][y]->color==playercolor)&&(fields[x][y]->type==PAWN) ;
}

bool Game::isPlayerFigure(QPoint p)
{
    return isPlayerFigure(p.x(),p.y()) ;
}

bool Game::isEnemyFigure(QPoint p)
{
    return isEnemyFigure(p.x(),p.y()) ;
}

