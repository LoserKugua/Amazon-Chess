#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <QVector>
#include "ChessBoardView.hpp"
#include "ChessGrid.hpp"
#include "../GameCore/Board.hpp"// 渲染棋盘用的。这样还能方便读档。

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);
    virtual ~ChessBoard();

protected:
    QGraphicsScene *scene;
    ChessBoardView *view;
    QVector<QVector<ChessGrid*> > boardGrids;// 存每个格子
    void setupChessBoard(const GameBoard &setBoard);
};

#endif // CHESSBOARD_HPP