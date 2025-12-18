#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDataStream>
#include "Board.hpp"

const int CheckDx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int CheckDy[8] = {0, -1, -1, -1, 0, 1, 1, 1};

class GameState {
protected:
    std::vector<ChessMove> HistoryMoveBlack;
    std::vector<ChessMove> HistoryMoveWhite;
    GameBoard Board;
    bool CurrentPlayer;// 0到黑1到白 这玩意还出诡异bug了
    bool GameOverTag;
    int gameTurns;
public:
    GameState();
    virtual ~GameState() = default;
    bool GetPlayer();// 获取当前下棋者
    void ChangePlayer(int player);// 轮换
    const GameBoard& GetBoard() const;
    void MoveBlack(ChessMove move);// 黑棋历史变更
    void MoveWhite(ChessMove move);// 白棋历史变更
    void ModifyBoard(ChessPosition pos, int value);// 直接改board
    void GameOver();// 游戏结束设置
    ChessMove Undo(); // 返回false悔棋失败

    void modifyTurns(int turns);
    int getTurns();

    int LoadSave(QString filePath);
    int GenerateSave(QString filePath);
};

#endif