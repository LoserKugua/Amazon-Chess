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
    bool GetPlayer() const;// 获取当前下棋者
    void ChangePlayer(bool player);// 轮换
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

class AIGameState : public GameState {
// 构造函数默认继承了
public:
    AIGameState(const GameState &state_2);
    AIGameState& operator = (const AIGameState &state); // 只用于AI 所以不带历史记录

    void getValidMoves(std::vector<ChessMove>& result);
    bool validPos(const ChessPosition &pos) const;// 判断越界和是不是空位
    void getValidPoses(const ChessPosition &pos, std::vector<ChessPosition>& result); // 获取所有可移动位置 皇后移动位
    void getValidArrows(const ChessPosition &pos, std::vector<ChessPosition>& result); // 获取所有可放障碍位 临近八方向

    void makeMove(const ChessMove &move);
    void undoMove(const ChessMove &move2);
    int getGameResult();// 1黑赢2白赢
};

#endif