#ifndef MCTS_HPP
#define MCTS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <chrono>
#include <queue>
#include <iomanip>
#include <random>
#include "../GameCore/GameState.hpp"

const int N = 8;
const int BOARD_SZ = N * N;
const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
const int EMPTY = 0;
const int BLACK = 1;
const int WHITE = 2;
const int ARROW = 3;
struct Point {
    int x, y;
};

struct Move { // 不using namespace std的好处 不然x1 y1直接被cmath创报错了
    int x0, y0; 
    int x1, y1; 
    int x2, y2; 
    bool is_null() const { // -1表示没移动
        return x0 == -1;
    }
};

// 游戏状态
struct MCTSGameState {
    int board[BOARD_SZ];
    int current_player;
    int black_queens[4];
    int white_queens[4];

    MCTSGameState();
    ~MCTSGameState() = default;
    void apply_move(const Move& m);
    bool valid_pos(const int &x, const int &y) const;
    void get_valid_moves(std::vector<Move>& moves) const;// 生成所有合法移动

    bool is_terminal() const;
};

struct Node {
    MCTSGameState state;
    Move move_from_parent;
    int parent_idx;
    std::vector<int> children;
    std::vector<Move> untried_moves;

    ~Node() = default;
    
    double wins;
    int visits;

    Node(const MCTSGameState& s, Move m, int parent);
    bool is_fully_expanded() const;
    bool is_terminal() const;
};

class MCTS {
public:
    MCTS(AIGameState initial_state, long long time_limit_ms = 0);
    ~MCTS() = default;
    int expand(int node_idx);
    void backpropagate(int node_idx, double val);
    int run_mcts();
    ChessMove findBestMove();

private:
    long long timeLimitMs; // 时间限制 (单位ms)
    MCTSGameState rootState;
    std::vector<Node> tree;
};

#endif