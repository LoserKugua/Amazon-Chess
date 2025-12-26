// 重新写的
// 第一次写因为封装过于臃肿 以及没用位板所以效率太低
// 加个接口转化一下

#include "MCTS.hpp"

const double C_EXPLORE = 0.5;
const double EPSILON = 1e-6;

double weights[3][3] = {{0.5, 1.0, 2.0}, 
                        {1.0, 3.0, 10.0},
                        {2.0, 1.0, 0.1} };
// moblity
// territory
// potential

int turn_count;

std::mt19937 rnd(std::random_device{}());

int idx(const int &x, const int &y) { 
        return x * N + y;
}

int get_x(const int &i) { 
    return i / N;
}

int get_y(const int &i) {
    return i % N; 
}

MCTSGameState::MCTSGameState() {
    memset(board, 0, sizeof(board));
    current_player = BLACK;
    // 黑棋: (2,0), (0,2), (0,5), (2,7) ->  16, 2, 5, 23
    int b_pos[4] = {16, 2, 5, 23};
    // 白棋: (5,0), (7,2), (7,5), (5,7) ->  40, 58, 61, 47
    int w_pos[4] = {40, 58, 61, 47};
    for(short i = 0; i < 4; ++i) {
        board[b_pos[i]] = BLACK;
        black_queens[i] = b_pos[i];
        board[w_pos[i]] = WHITE;
        white_queens[i] = w_pos[i];
    }
}

void MCTSGameState::apply_move(const Move &m) {
    if (m.is_null()) return;
    int start = idx(m.x0, m.y0);
    int end = idx(m.x1, m.y1);
    int arrow = idx(m.x2, m.y2);
    board[start] = EMPTY;
    board[end] = current_player;
    // 更新缓存的皇后位置
    int* queens = (current_player == BLACK) ? black_queens : white_queens;
    for(short i = 0; i < 4; ++i) {
        if (queens[i] == start) {
            queens[i] = end;
            break;
        }
    }
    board[arrow] = ARROW;
    current_player = (current_player == BLACK) ? WHITE : BLACK;
}

bool MCTSGameState::valid_pos(const int &x, const int &y) const {
    return x >= 0 && x < N && y >= 0 && y < N && board[idx(x, y)] == EMPTY;
}

void MCTSGameState::get_valid_moves(std::vector<Move>& moves) const {
    moves.reserve(200);
    const int* queens = (current_player == BLACK) ? black_queens : white_queens;
    for (short i = 0; i < 4; ++i) {
        int start_idx = queens[i];
        int sx = get_x(start_idx);
        int sy = get_y(start_idx);
        for (short j = 0; j < 8; ++j) {
            int cx = sx + dx[j];
            int cy = sy + dy[j];
            while (valid_pos(cx, cy)) {
                for (short k = 0; k < 8; ++k) {
                    int ax = cx + dx[k];
                    int ay = cy + dy[k];
                    while (ax >= 0 && ax < N && ay >= 0 && ay < N) {
                        int a_idx = idx(ax, ay);
                        if (board[a_idx] == EMPTY || a_idx == start_idx) {
                            moves.push_back({sx, sy, cx, cy, ax, ay});
                        }
                        else break; // 被阻挡
                        ax += dx[k];
                        ay += dy[k];
                    }
                    cx += dx[j];
                    cy += dy[j];
                }
            }
        }
    }
}

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

bool MCTSGameState::is_terminal() const { // 判断游戏结束没有
    const int* queens = (current_player == BLACK) ? black_queens : white_queens;
    for(short i = 0; i < 4; ++i) {
        int r = get_x(queens[i]);
        int c = get_y(queens[i]);
        for(short d = 0; d < 8; ++d) {
            int nr = r + dx[d], nc = c + dy[d];
            if (valid_pos(nr, nc)) return false;
        }
    }
    return true;    
}

double pow_2(int x) { // 2的负x次方
    switch(x) {
        case 0:
            return 1;
        case 1:
            return 0.5;
        case 2:
            return 0.25;
        case 3:
            return 0.125;
        case 4:
            return 0.0625;
        case 5:
            return 0.03125;
        case 6:
            return 0.015625;
        case 7:
            return 0.0078125;
        default:
            return 0;
    }
    return 0;
}

double evaluate_state(const MCTSGameState& state, int root_player) { // 估价函数
    int dist_map[2][BOARD_SZ];

    int my_mobility = 0;
    int op_mobility = 0;
    
    auto count_mob = [&](int player) -> int {
        int m = 0;
        const int* q = (player == BLACK) ? state.black_queens : state.white_queens;
        for(short i = 0; i < 4; ++i) {
            int r = get_x(q[i]), c = get_y(q[i]);
            for(short d = 0; d < 8; ++d) {
                int nr = r + dx[d], nc = c + dy[d];
                // 仅计算第一步周围的格子，近似代表机动性
                while(state.valid_pos(nr, nc)) {
                    m++; 
                    nr += dx[d]; nc += dy[d];
                }
            }
        }
        return m;
    };

    my_mobility = count_mob(root_player);
    op_mobility = count_mob((root_player == BLACK) ? WHITE : BLACK);

    // 领地 Territory: 多源 BFS
    // 0 -> root_player, 1 -> opponent
    for(short i = 0; i < BOARD_SZ; ++i) {
        dist_map[0][i] = 999;
        dist_map[1][i] = 999;
    }

    std::queue<std::pair<int, int> > q;

    const int* my_q = (root_player == BLACK) ? state.black_queens : state.white_queens;
    const int* op_q = (root_player == BLACK) ? state.white_queens : state.black_queens;

    for(short i = 0; i < 4; ++i) {
        dist_map[0][my_q[i]] = 0;
        q.push({my_q[i], 0});
        dist_map[1][op_q[i]] = 0;
        q.push({op_q[i], 1});
    }

    while(!q.empty()) {
        std::pair<int, int> curr_pair = q.front();
        q.pop();
        int curr = curr_pair.first;
        int owner = curr_pair.second;

        int d = dist_map[owner][curr];
        int r = curr / N;
        int c = curr % N;

        for(short k = 0; k < 8; ++k) {
            int nr = r + dx[k];
            int nc = c + dy[k];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                int nidx = nr * N + nc;
                if(state.board[nidx] == EMPTY) {
                    if(dist_map[owner][nidx] > d + 1) {
                        dist_map[owner][nidx] = d + 1;
                        q.push({nidx, owner});
                    }
                }
            }
        }
    }
    
    double potential = 0;
    
    // 指数计算势能
    for(short i = 0; i < BOARD_SZ; ++i) {
        if(state.board[i] != EMPTY) continue;
        int d1 = dist_map[0][i];
        int d2 = dist_map[1][i];
        potential += pow_2(d1) - pow_2(d2);
    }
    
    // queenmove计算territory
    int my_territory = 0;
    int op_territory = 0;

    for(short i = 0; i < 4; ++i) {
        dist_map[0][my_q[i]] = 0;
        q.push({my_q[i], 0});
        dist_map[1][op_q[i]] = 0;
        q.push({op_q[i], 1});
    }

    while(!q.empty()) {
        std::pair<int, int> curr_pair = q.front();
        q.pop();
        int curr = curr_pair.first;
        int owner = curr_pair.second;

        int d = dist_map[owner][curr];
        int r = curr / N;
        int c = curr % N;

        for(short k = 0; k < 8; ++k) {
            int nr = r + dx[k];
            int nc = c + dy[k];
            while(state.valid_pos(nr, nc)) {
                int nidx = nr * N + nc;
                if(dist_map[owner][nidx] > d + 1) {
                    dist_map[owner][nidx] = d + 1;
                    q.push({nidx, owner});
                }
                nr += dx[k];
                nc += dy[k];
            }
        }
    }

    for(short i = 0; i < BOARD_SZ; ++i) {
        if(state.board[i] != EMPTY) continue;
        int d1 = dist_map[0][i];
        int d2 = dist_map[1][i];
        if (d1 < d2) ++my_territory;
        else if (d2 < d1) ++op_territory;
    }

    // 分阶段决策(
    int empty_cnt = 56 - ((turn_count - 1) << 1);
    
    double w_mob, w_terr, w_pot;
    // 空格多 -> 开局/中局: 侧重机动性
    // 空格少 -> 残局: 侧重领地
    if (empty_cnt > 45) { 
        w_mob = weights[0][0];
        w_terr = weights[1][0];
        w_pot = weights[2][0];
    } else if (empty_cnt > 20) { 
        w_mob = weights[0][1];
        w_terr = weights[1][1];
        w_pot = weights[2][1];
    } else { 
        w_mob = weights[0][2];
        w_terr = weights[1][2];
        w_pot = weights[2][2];
    }
    double score = w_terr * (my_territory - op_territory) + 
                   w_mob * (my_mobility - op_mobility) + 
                   w_pot * potential;
    // 映射到 [0, 1]
    return sigmoid(score * 0.25); 
}

Node::Node(const MCTSGameState& s, Move m, int parent) 
    : state(s), move_from_parent(m), parent_idx(parent), wins(0), visits(0) {
    state.get_valid_moves(untried_moves);
    shuffle(untried_moves.begin(), untried_moves.end(), rnd);
}

bool Node::is_fully_expanded() const {
    return untried_moves.empty();
}

bool Node::is_terminal() const {
    return children.empty() && untried_moves.empty() && state.is_terminal();
}

MCTS::MCTS(AIGameState initial_state, long long time_limit_ms) {
    timeLimitMs = time_limit_ms;
    for(short i = 0; i <= 7; ++i) {
        for(short j = 0; j <= 7; ++j) {
            ChessPosition pos = {i, j};
            if (initial_state.GetBoard().IsBlack(pos)) {
                rootState.board[idx(i, j)] = BLACK;
            }
            else if (initial_state.GetBoard().IsWhite(pos)) {
                rootState.board[idx(i, j)] = WHITE;
            }
            else if (initial_state.GetBoard().IsArrow(pos)) {
                rootState.board[idx(i, j)] = ARROW;
            }
            else rootState.board[idx(i, j)] = EMPTY;
        }
    }
    rootState.current_player = initial_state.GetPlayer();
    ++rootState.current_player;
    for(short i = 0; i <= 3; ++i) {
        ChessPosition pos = initial_state.GetBoard().GetBlackPos(i);
        rootState.black_queens[i] = idx(pos.x, pos.y);
    }
    for(short i = 0; i <= 3; ++i) {
        ChessPosition pos = initial_state.GetBoard().GetWhitePos(i);
        rootState.white_queens[i] = idx(pos.x, pos.y);
    }
}

int MCTS::expand(int node_idx) { // 扩展
    if (tree[node_idx].untried_moves.empty()) return -1; // 无法扩展

    Move m = tree[node_idx].untried_moves.back();
    tree[node_idx].untried_moves.pop_back();

    MCTSGameState next_state = tree[node_idx].state;
    next_state.apply_move(m);

    tree.emplace_back(next_state, m, node_idx);
    int child_idx = (int)tree.size() - 1;
    tree[node_idx].children.push_back(child_idx);

    return child_idx;
}

void MCTS::backpropagate(int node_idx, double val) { // MCTS: 反向传播
    while (node_idx != -1) {
        Node& node = tree[node_idx];
        ++node.visits;
        node.wins += val; // val 始终是相对于 Root 玩家的胜率
        node_idx = node.parent_idx;
    }
}

int MCTS::run_mcts() {
    clock_t start_time = clock();
    tree.clear();
    // 根节点
    tree.emplace_back(rootState, Move{-1, -1, -1, -1, -1, -1}, -1);
    int root_player = rootState.current_player;
    int iter = 0;
    while (true) {
        if(!(iter & 127)) { // 128次迭代卡时一次
            if (clock() - start_time > timeLimitMs) {
                // cout << elapsed.count() << '\n';
                break;
            }
        }
        ++iter;
        // 1. 选择 (Selection)
        int curr_idx = 0;
        while(tree[curr_idx].is_fully_expanded() && !tree[curr_idx].children.empty()) {
            Node& node = tree[curr_idx];
            int best_child = -1;
            double best_val = -1e9; // Maximize for Root
            double min_val = 1e9;   // Minimize for Opponent
            // Minimax UCT:
            // 如果该节点轮到 Root 走，他会选择 UCT 最大的子节点
            // 如果该节点轮到 Opponent 走，他会选择 UCT 最小的子节点
            bool is_root_turn = (node.state.current_player == root_player);
            for (int child_idx : node.children) {
                Node& child = tree[child_idx];
                double q = child.wins / (child.visits + EPSILON); // 平均胜率 (Root视角)
                double u = C_EXPLORE * sqrt(log(node.visits + 1) / (child.visits + EPSILON));
                if (is_root_turn) {
                    // 我方回合：选择 Q + U 最大
                    double score = q + u;
                    if (score > best_val) {
                        best_val = score;
                        best_child = child_idx;
                    }
                }
                else {
                    // 敌方回合：对手会最小化我的胜率，但我需要探索，所以是 Q - U 最小
                    // 使用 Minimax: 寻找 min(q - u)
                    double score = q - u; 
                    if (score < min_val) {
                        min_val = score;
                        best_child = child_idx;
                    }
                }
            }   
            if (best_child == -1) break;
            curr_idx = best_child;
        }
        // 2. 扩展  Expansion
        if (!tree[curr_idx].is_terminal() && !tree[curr_idx].is_fully_expanded()) {
            int new_child = expand(curr_idx);
            if (new_child != -1) curr_idx = new_child;
        }
        // 3. 静态评估 替代 Simulation
        double val = evaluate_state(tree[curr_idx].state, root_player);
        // 4. 反向传播  Backpropagation
        backpropagate(curr_idx, val);
    }

    // 最终选择：访问次数最多的子节点 (Robust Child)
    Node& root = tree[0];
    int best_child = -1;
    int max_visits = -1;
    
    for (int child_idx : root.children) {
        if (tree[child_idx].visits > max_visits) {
            max_visits = tree[child_idx].visits;
            best_child = child_idx;
        }
    }
    return best_child;
}

ChessMove MCTS::findBestMove() {
    int best_node = run_mcts();
    Move move;
    if (best_node != -1) {
        move = tree[best_node].move_from_parent;
    }
    else {
        std::vector<Move> moves;
        rootState.get_valid_moves(moves);
        if(!moves.empty()) move = moves[0];
        else move = {-1, -1, -1, -1, -1, -1};
    }
    return (ChessMove) {(ChessPosition){move.x0, move.y0}, (ChessPosition){move.x1, move.y1}, (ChessPosition){move.x2, move.y2}};
}