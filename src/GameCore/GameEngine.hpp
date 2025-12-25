#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "GameState.hpp"

class GameEngine
{
private:
    GameState CurrentState;
public:
    GameEngine();
    ~GameEngine();

    bool GetPlayer();// 再敲一遍 懒得重整合了
    void ChangePlayer(bool player);// 轮换
    const GameState& GetState();

    bool ValidPos(const ChessPosition &pos) const;// 判断越界和是不是空位
    void ModifyBoard(ChessPosition pos, int value);
    void HistoryPush(ChessMove move);
    void ModifyState(GameState state);

    void modifyTurns(int turns);
    int getTurns();

    std::vector<ChessPosition> GetValidMoves(const ChessPosition &pos) const;// 获取所有可移动位置 皇后移动位
    std::vector<ChessPosition> GetValidArrows(const ChessPosition &pos) const;// 获取所有可放障碍位 临近八方向

    int GameOver();// 0没结束,1是黑赢,2是白赢

    int GenerateSave(QString filePath);// 存档
    int LoadSave(QString filePath); // 读档
    ChessMove Undo(); // 悔棋
};

#endif