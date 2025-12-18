#ifndef BOARD_HPP
#define BOARD_HPP
/*
其实我的习惯是从1开始算坐标
为了方便编码啥的还是从0开始吧
BoardState 0空 1黑棋 2白棋 3障碍
本project对坐标编码符合：(主要是GUI这么排版)
|-------二维数组里层(y)
|
|
|
二维数组外层(x)
(x, y)
为了封装性，接下来你会看到最臃肿的public
*/
#include <cstring>
#include "move.hpp"

class GameBoard
{
private:
    short BoardState[8][8];
    ChessPosition BlackChess[4], WhiteChess[4];
public:
    GameBoard();
    ~GameBoard() = default;
    // 判断是不是黑棋 白棋 空棋
    // 主要是懒得访问private变量()
    bool IsBlack(ChessPosition pos) const;
    bool IsWhite(ChessPosition pos) const;
    bool IsArrow(ChessPosition pos) const;
    bool IsAvailable(ChessPosition pos) const;
    // 取得对应编号
    int GetBlackIndex(ChessPosition pos) const;
    int GetWhiteIndex(ChessPosition pos) const;
    // 移动黑白棋 通过move.from获取编号就行
    void MoveBlack(ChessMove move);
    void MoveWhite(ChessMove move);
    void modify(ChessPosition pos, int value);// 没辙了
    void modifyBlack(ChessPosition pos, int index);
    void modifyWhite(ChessPosition pos, int index);
    // 取得编号对应位置
    ChessPosition GetBlackPos(int index) const;
    ChessPosition GetWhitePos(int index) const;
    // debug
    void BoardDebug() const;
    void BlackChessDebug() const;
    void WhiteChessDebug() const;
    friend bool operator == (const GameBoard &board1, const GameBoard &board2) {
        for(int i = 0; i <= 7; ++i) {
            for(int j = 0; j <= 7; ++j) {
                if(board1.BoardState[i][j] != board2.BoardState[i][j]) {
                    return false;
                }
            }
        }
        for(int i = 0; i <= 3; ++i) {
            if(!(board1.BlackChess[i] == board2.BlackChess[i])) {
                return false;
            }
            if(!(board2.WhiteChess[i] == board2.WhiteChess[i])) {
                return false;
            }
        }
        return true;
    }
};

#endif // BOARD_HPP