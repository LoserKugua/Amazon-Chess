#ifndef MOVE_HPP
#define MOVE_HPP

#include <QDebug>

struct ChessPosition {
    int x, y;
    friend bool operator == (const ChessPosition &pos1, const ChessPosition &pos2) {
        return pos1.x == pos2.x && pos1.y == pos2.y;
    }
    friend bool operator != (const ChessPosition &pos1, const ChessPosition &pos2) {
        return pos1.x != pos2.x || pos1.y != pos2.y;
    }
};

class ChessMove
{
private:
    ChessPosition From, To, Arrow;// 从哪来到哪去 障碍放哪
public:
    ChessMove();
    ChessMove(const ChessPosition &pos1, const ChessPosition &pos2, const ChessPosition &pos3);
    ~ChessMove() = default;
    const ChessPosition& GetFrom() const;
    const ChessPosition& GetTo() const;
    const ChessPosition& GetArrow() const;
    void ChangeFrom(const ChessPosition &pos);
    void ChangeTo(const ChessPosition &pos);
    void ChangeArrow(const ChessPosition &pos);
    // debug
    void MoveDebug();
    friend bool operator == (const ChessMove &move1, const ChessMove &move2) {
        if(move1.From != move2.From) return false;
        if(move1.To != move2.To) return false;
        if(move1.Arrow != move2.Arrow) return false;
        return true;
    }
};

#endif // MOVE_H