#ifndef TWOPLAYER_HPP
#define TWOPLAYER_HPP

#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include "ChessBoard.hpp"
#include "../GameCore/GameEngine.hpp"

class TwoPlayer : public ChessBoard {
    Q_OBJECT

protected:
    QLabel *modeText; // 模式信息显示
    ChessPosition chosenChess; // 被选中的棋子
    std::vector<ChessPosition> placePres;
    GameEngine* Engine_2; // 游戏引擎
    QString modeTextBlack; // 游戏模式提示的
    QString modeTextWhite;
    QString modeTextEnd; // 游戏结束label
    
    int placeStage; // 放置阶段:0没选棋子 1准备落棋子 2:放箭!
    int gameTurns; // 游戏进行轮数
    ChessMove currentMove;
    ChessMove tipsMove; // 不会让几次MCTS跑出来结果不一样的()
    
    bool isTiping; // 提示中
    bool isGameOver; // 游戏结束
    bool gameSaved; // 判断存档没有

    // 单人模式重写
    virtual void moreMove(); // 读档之后看看轮到人机下是不是
    virtual void moreUndo();
    virtual bool gameStart(); // 单人模式棋子单下一次不能撤

    void buttonsInit(); // 按钮的初始化
    void buttonSetable(bool mode); // 0是全部禁用 1是全部启用
    void gameOverJudge(); // 游戏结束判断 弹messagebox
    void finishMoveMsg(); // 把这步棋子走完的提示

    void boardClickResponse(int col, int row); // 响应点击事件
    // 和placepres一起承担 待放置位和arrow位渲染
    // 模式0是placePre渲染 模式1是arrowPre渲染 模式2是empty渲染
    void placeRender(int renderMode);
    void reRender();// 棋盘重新渲染
    void chessChoosing(int col, int row); // placeStage 0->1
    void chessPlacing(int col, int row); // placeStage 1->2
    void arrowPlacing(int col, int row); // placeStage 2->0

public:
    QPushButton *exitButton; // 退出按钮
    QPushButton *saveButton; // 存档按钮
    QPushButton *loadButton; // 读档按钮
    QPushButton *undoButton; // 悔棋按钮
    QPushButton *tipsButton; // 提示按钮
    QPushButton *settingsButton; // 设置按钮
    int saveMessage(); // 存档提示
    int suffixMessage(); // 文件类别提示
    bool isGameSaved(); // gameSaved的访问接口
    explicit TwoPlayer(QWidget *parent = nullptr);
    virtual ~TwoPlayer();
    
public slots:
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_undoButton_clicked();
    void on_tipsButton_clicked();
};

#endif