#include "move.hpp"

ChessMove::ChessMove() {
    From.x = From.y = 0;
    To.x = To.y = 0;
    Arrow.x = Arrow.y = 0;
}

ChessMove::ChessMove(const ChessPosition &pos1, const ChessPosition &pos2, const ChessPosition &pos3) {
    From = pos1;
    To = pos2;
    Arrow = pos3;
}

const ChessPosition& ChessMove::GetFrom() const {
    return From;
}

const ChessPosition& ChessMove::GetTo() const {
    return To;
}

const ChessPosition& ChessMove::GetArrow() const {
    return Arrow;
}

void ChessMove::ChangeFrom(const ChessPosition &pos) {
    From = pos;
}

void ChessMove::ChangeTo(const ChessPosition &pos) {
    To = pos;
}

void ChessMove::ChangeArrow(const ChessPosition &pos) {
    Arrow = pos;
}

void ChessMove::MoveDebug() {
    qDebug() << "    from to arrow";
    qDebug() << "Move" << From.x << ' ' << From.y << ' ' << To.x << ' ' << To.y << ' ' << Arrow.x << ' ' << Arrow.y;
}