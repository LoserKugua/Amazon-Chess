#ifndef MCTS_HPP
#define MCTS_HPP

#include "../GameCore/GameState.hpp"
#include <random>
#include <chrono>

class AIGameState : public GameState {
// 构造函数默认继承了
public:
    AIGameState(const GameState &state_2);
    AIGameState& operator = (const AIGameState &state); // 只用于AI 所以不带历史记录

    std::vector<ChessMove> getValidMoves();
    bool validPos(const ChessPosition &pos) const;// 判断越界和是不是空位
    void getValidPoses(const ChessPosition &pos, std::vector<ChessPosition>& result); // 获取所有可移动位置 皇后移动位
    void getValidArrows(const ChessPosition &pos, std::vector<ChessPosition>& result); // 获取所有可放障碍位 临近八方向

    void makeMove(const ChessMove &move);
    void undoMove(const ChessMove &move2);
    int getGameResult();// 1黑赢2白赢
};

struct MCTSNode { // MCTS 节点结构体
    AIGameState state;
    ChessMove move;
    MCTSNode* parent; // 父节点
    std::vector<MCTSNode*> children; // 子节点
    
    int visits; // 访问次数
    double wins; // 胜利次数
    
    MCTSNode(const AIGameState& state_2, ChessMove move_2 = ChessMove(), MCTSNode* parent_2 = nullptr);
    ~MCTSNode();

    std::vector<ChessMove> untriedMoves; // 未尝试过的合法移动
    bool isExpanded; // 标记是否已经扩展过 (所有子节点已生成)
    bool isTerminal() { // 判断游戏是否结束
        return state.getGameResult() != 0;
    }

    double getUCTValue() const; // 获取 UCT 值 
    MCTSNode* selectBestChild(); // 选择最佳子节点 (基于 UCT 值)
    MCTSNode* expand(std::mt19937& randomEngine); // 扩展一个新节点
    int simulate(std::mt19937& randomEngine); // 模拟到游戏结束
    void backpropagate(int result); // 回溯更新节点信息(反向传播)
};

class MCTS {
public:
    MCTS(const AIGameState& initial_state, int num_simulations, long long time_limit_ms = 0);
    ~MCTS();
    ChessMove findBestMove();

private:
    MCTSNode* root; // MCTS 树的根节点
    int totalSimulations; // 最大模拟次数
    long long timeLimitMs; // 时间限制 (单位ms)
    std::mt19937 randomEngine;
};

#endif