#ifndef GAME_H
#define GAME_H

enum PlayerColor{WHITE, BLACK};
enum PieceType{PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

const int SZ = 8 ;

// Запись фигуры
struct Piece {
   PieceType type ; // Тип
   PlayerColor color ; // Цвет
} ;

// Класс игры
class Game : public QObject
{
    Q_OBJECT
private:
    // Добавление фигуры
    void addPiece(PlayerColor color, PieceType type, int x, int y) ;

    // Текущий цвет игрока
    PlayerColor playercolor ;
    PlayerColor enemycolor ;

    // Методы для построения ходов
    bool buildMyAttack(QList<QPoint> &lst, int x, int y);

    void buildMyBishopAttack(QList<QPoint> &lst, int x, int y);

    void buildMyRookAttack(QList<QPoint> &lst, int x, int y);

    bool buildEnemyAttack(QList<QPoint> &lst, int x, int y);

    void buildEnemyBishopAttack(QList<QPoint> &lst, int x, int y);

    void buildEnemyRookAttack(QList<QPoint> &lst, int x, int y);

    // Построение клеток под атакой врага
    QList<QPoint> buildEnemyAttacked() ;

public:

    // Массив клеток доски
    Piece *** fields ;
    // Метка хода (свой или чужой)
    bool mystep ;

    explicit Game(PlayerColor playercolor);

    // Заполнение представления
    void fillBoard() ;

    // Получение доступных ходов
    QList<QPoint> avaibleMoves(int x, int y) ;

    bool isCastling(int x1, int y1, int x2, int y2) ;

    // Выполнить ход, указав, ходит ли человек или сокет
    bool makeMove(int x1, int y1, int x2, int y2, bool playerstep) ;

    bool makeCastling(int x1, int y1, int x2, int y2) ;

    void applyCastling(int x1, int x2, int xKing, int xRook, bool isplayer) ;

    PlayerColor getPlayerColor() ;

    // Получение параметров доски по точкам
    bool isPlayerFigure(int x, int y) ;

    bool isEnemyFigure(int i, int j) ;

    bool isEmpty(int x, int y) ;

    bool isPlayerFigure(QPoint p) ;

    bool isEnemyFigure(QPoint p) ;

    bool isEnemyPesh(int x, int y) ;

    bool isPlayerPesh(int x, int y) ;

    bool isEmpty(QPoint p) ;
signals:
    // Сигналы на заполнение представления
    void clearChess() ;
    void addToChess(PlayerColor color, PieceType type, int i, int j) ;
    // И на отправу хода через сокет
    void sendStepOverSocket(int x1, int y1, int x2, int y2) ;
    void sendRockOverSocket(int ix1, int x2, int xKing, int xRook) ;

public slots:
};



#endif // GAME_H
