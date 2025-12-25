#include "Board.hpp"
#include <QDebug>

GameBoard::GameBoard()
{
    std::memset(BoardState, 0, sizeof(BoardState));
    // blackchess
    BoardState[2][0] = 1;
    BoardState[0][2] = 1;
    BoardState[0][5] = 1;
    BoardState[2][7] = 1;
    BlackChess[0] = (ChessPosition) {2, 0};
    BlackChess[1] = (ChessPosition) {0, 2};
    BlackChess[2] = (ChessPosition) {0, 5};
    BlackChess[3] = (ChessPosition) {2, 7};

    // whitechess
    BoardState[5][0] = 2;
    BoardState[7][2] = 2;
    BoardState[7][5] = 2;
    BoardState[5][7] = 2;
    WhiteChess[0] = (ChessPosition) {5, 0};
    WhiteChess[1] = (ChessPosition) {7, 2};
    WhiteChess[2] = (ChessPosition) {7, 5};
    WhiteChess[3] = (ChessPosition) {5, 7};
}

bool GameBoard::IsBlack(ChessPosition pos)  const {
    return BoardState[pos.x][pos.y] == 1;
}

bool GameBoard::IsWhite(ChessPosition pos) const {
    return BoardState[pos.x][pos.y] == 2;
}

bool GameBoard::IsArrow(ChessPosition pos) const {
    return BoardState[pos.x][pos.y] == 3;
}

bool GameBoard::IsAvailable(ChessPosition pos)  const {
    return BoardState[pos.x][pos.y] == 0;
}

int GameBoard::GetBlackIndex(ChessPosition pos) const {
    if(pos == BlackChess[0]) return 0;
    if(pos == BlackChess[1]) return 1;
    if(pos == BlackChess[2]) return 2;
    if(pos == BlackChess[3]) return 3;
    return 4;
}

int GameBoard::GetWhiteIndex(ChessPosition pos) const {
    if(pos == WhiteChess[0]) return 0;
    if(pos == WhiteChess[1]) return 1;
    if(pos == WhiteChess[2]) return 2;
    if(pos == WhiteChess[3]) return 3;
    return 4;
}

void GameBoard::MoveBlack(ChessMove move) {
    int index = GetBlackIndex(move.GetFrom());
    BlackChess[index] = move.GetTo();
}

void GameBoard::MoveWhite(ChessMove move) {
    int index = GetWhiteIndex(move.GetFrom());
    WhiteChess[index] = move.GetTo();
}

void GameBoard::modify(ChessPosition pos, int value) {
    BoardState[pos.x][pos.y] = value;
}

void GameBoard::modifyBlack(ChessPosition pos, int index) {
    BlackChess[index] = pos;
}

void GameBoard::modifyWhite(ChessPosition pos, int index) {
    WhiteChess[index] = pos;
}

ChessPosition GameBoard::GetBlackPos(int index) const {
    return BlackChess[index];
}

ChessPosition GameBoard::GetWhitePos(int index) const {
    return WhiteChess[index];
}

void GameBoard::BoardDebug() const {
    qDebug() << "now board:";
    for(int i = 0; i <= 7; ++i) {
        qDebug() << BoardState[i][0] << ' ' <<  BoardState[i][1] << ' ' << BoardState[i][2] << ' ' << BoardState[i][3] << ' ' << BoardState[i][4] << ' ' << BoardState[i][5] << ' ' << BoardState[i][6] << ' ' << BoardState[i][7];
    }
    qDebug() << "black Chess:";
    for(int i = 0; i <= 3; ++i) {
        qDebug() << BlackChess[i].x << ' ' << BlackChess[i].y;
    }
    qDebug() << "white Chess:";
    for(int i = 0; i <= 3; ++i) {
        qDebug() << WhiteChess[i].x << ' ' << WhiteChess[i].y;
    }
}

void GameBoard::BlackChessDebug() const {
    qDebug() << "black chess:";
    for(int i = 0; i <= 3; ++i) {
        qDebug() << BlackChess[i].x << ' ' << BlackChess[i].y;
    }
}

void GameBoard::WhiteChessDebug() const {
    qDebug() << "white chess:";
    for(int i = 0; i <= 3; ++i) {
        qDebug() << WhiteChess[i].x << ' ' << WhiteChess[i].y;
    }
}