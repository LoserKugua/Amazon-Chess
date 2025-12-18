#include "ChessBoard.hpp"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 640, 640);

    view = new ChessBoardView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFixedSize(640 + 2, 640 + 2);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->move(80, 80);
}

ChessBoard::~ChessBoard() {
}

void ChessBoard::setupChessBoard(const GameBoard &setBoard)
{
    const int BOARD_SIZE = 8; // 棋盘维度
    const int SQUARE_SIZE = 80; // 每个格子的大小 (像素)

    // 定义两种颜色，用于交替绘制格子
    QColor lightColor(240, 217, 181); // 米白色
    QColor darkColor(181, 136, 99);   // 棕色
    boardGrids.resize(BOARD_SIZE);
    for (int row = 0; row < BOARD_SIZE; ++row) {
        boardGrids[row].resize(BOARD_SIZE);
        for (int col = 0; col < BOARD_SIZE; ++col) {
            // 交替画格子而已
            qreal x = col * SQUARE_SIZE;
            qreal y = row * SQUARE_SIZE;
            QColor currentColor = ((row + col) % 2 == 0) ? lightColor : darkColor;

            // QGraphicsRectItem四个参数跟SetGeoMetry是一样的 左上角 大小
            ChessGrid *square = new ChessGrid(x, y, SQUARE_SIZE, SQUARE_SIZE);
            square->setColor(currentColor);
            scene->addItem(square); // 将格子添加到场景中
            boardGrids[row][col] = square;
        }
    }

    for(int i = 0; i <= 7; ++i) {
        for(int j = 0; j <= 7; ++j) {
            ChessPosition posn = {i, j};
            if(setBoard.IsBlack(posn)) {
                boardGrids[posn.x][posn.y]->setState("black");
            }
            else if(setBoard.IsWhite(posn)) {
                boardGrids[posn.x][posn.y]->setState("white");
            }
            else if(setBoard.IsArrow(posn)) {
                boardGrids[posn.x][posn.y]->setState("arrow");
            }
        }
    }

    // qDebug() << "chess board setup complete.";
}