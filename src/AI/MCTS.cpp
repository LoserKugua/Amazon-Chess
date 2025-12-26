#include "MCTS.hpp"
#include <limits>
#include <algorithm>
#include <cmath>

double C_PUCT = 1.414; // UCT评估参数
int simul_times = 3;

MCTSNode::MCTSNode(const AIGameState& state_2, ChessMove move_2, MCTSNode* parent_2) 
    : state(state_2), move(move_2), parent(parent_2), visits(0), wins(0.0), isExpanded(false) {

}

MCTSNode::~MCTSNode() {
    for (MCTSNode* child : children) {
        delete child;
    }
}

double MCTSNode::getUCTValue() const { // 计算 UCT 值
    if (visits == 0) {
        return std::numeric_limits<double>::infinity(); // 未访问过的节点优先选择
    }
    // UCT = 收益 / 访问次数 + 探索项
    // 探索项 (C_PUCT * sqrt(log(visits_of_parent) / visits_of_this_node))
    return wins / visits + 
            C_PUCT * std::sqrt(std::log(static_cast<double>(parent->visits)) / static_cast<double>(visits));
}

MCTSNode* MCTSNode::selectBestChild() { // 选择最佳子节点
    MCTSNode* best_child = nullptr;
    double max_uct_value = -1.0;
    for (MCTSNode* child : children) {
        double uct_val = child->getUCTValue();
        if (uct_val > max_uct_value) {
            max_uct_value = uct_val;
            best_child = child;
        }
    }
    return best_child;
}

MCTSNode* MCTSNode::expand(std::mt19937& randomEngine) { // 扩展一个子节点
    if (untriedMoves.empty()) { // 移动完了 扩展不了
        return this;
    }
    // **随机选择一个未尝试过的移动（可优化）
    ChessMove move_to_expand = untriedMoves.back();
    untriedMoves.pop_back();
    // 这里就是把移动应用到子节点上
    AIGameState new_state = state;
    new_state.makeMove(move_to_expand);
    MCTSNode* new_child = new MCTSNode(new_state, move_to_expand, this);
    if (!new_child->isTerminal()) { // 如果子节点不是终止状态，才需要生成合法移动
        new_child->state.getValidMoves(new_child->untriedMoves);
        std::shuffle(new_child->untriedMoves.begin(), new_child->untriedMoves.end(), randomEngine);
    }
    children.emplace_back(new_child);
    if (untriedMoves.empty()) { // 标记已扩展
        isExpanded = true;
    }
    return new_child;
}

// 从当前局面开始随机模拟
int MCTSNode::simulate(std::mt19937& randomEngine)  {
    AIGameState current_sim_state = this->state;
    int game_result = current_sim_state.getGameResult();
    std::vector<ChessMove> legal_moves;
    while (game_result == 0) { // 游戏未结束    
        current_sim_state.getValidMoves(legal_moves);
        if (legal_moves.empty()) { // 一般用不到
            game_result = (current_sim_state.GetPlayer() == 0) ? 2 : 1;// 0是都没赢1是黑赢2是白赢
            break;
        }
        // 随机选择一个合法移动
        std::uniform_int_distribution<size_t> distrib(0, legal_moves.size() - 1);
        ChessMove random_move = legal_moves[distrib(randomEngine)];
        current_sim_state.makeMove(random_move);
        game_result = current_sim_state.getGameResult();
    }
    return game_result;
}

// 回溯更新父节点的访问次数和胜利次数
void MCTSNode::backpropagate(int result) {
    MCTSNode* current = this;
    while (current != nullptr) {
        current->visits++;
        if (current->state.GetPlayer() + 1 == result) {
             current->wins += 1.0;
        }
        current = current->parent;
    }
}

MCTS::MCTS(const AIGameState& initial_state, long long time_limit_ms)
    : timeLimitMs(time_limit_ms) {
    root = new MCTSNode(initial_state);
    randomEngine.seed(std::random_device{}());
}

MCTS::~MCTS() {
    delete root;
}

ChessMove MCTS::findBestMove() { // MCTS 搜索主循环
    auto start_time = std::chrono::high_resolution_clock::now();
    // 生成根节点合法移动
    if (root->untriedMoves.empty() && !root->isTerminal()) {
        root->state.getValidMoves(root->untriedMoves);
        std::shuffle(root->untriedMoves.begin(), root->untriedMoves.end(), randomEngine);
    }
    while (true) {
        if (timeLimitMs > 0) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
            if (duration.count() >= timeLimitMs) {
                // qDebug() << simulations_done << "次迭代: " << duration.count() << "ms";
                break;
            }
        }
        MCTSNode* node = root;
        // 选择: 从根节点开始，通过UCT值选择最佳子节点，直到遇到未完全扩展的节点或叶子节点
        while (!node->isTerminal() && node->untriedMoves.empty() && !node->children.empty()) {
            node = node->selectBestChild();
        }
        // 扩展: 如果当前节点还有未尝试过的移动，则扩展一个新子节点
        MCTSNode* leaf = node;
        if (!node->isTerminal() && !node->untriedMoves.empty()) {
            leaf = node->expand(randomEngine); // 扩展一个新节点
        }
        // 模拟: 从新扩展的节点或选中的叶子节点进行随机模拟到游戏结束
        int result = leaf->simulate(randomEngine);
        // 回溯: 根据模拟结果更新路径上所有节点的访问次数和胜利次数
        leaf->backpropagate(result);
        for(int i = 1; i <= simul_times; ++i) {
            result = leaf->simulate(randomEngine);
            leaf->backpropagate(result);
        }
    }

    MCTSNode* best_child = nullptr;
    int max_visits = -1;
    double max_win_rate = -1.0; 
    if (root->children.empty()) {
        return ChessMove(); // 返回一个无效移动
    }
    for (MCTSNode* child : root->children) {
        if (!child->visits) continue;
        double current_win_rate = 1.0 - child->wins / child->visits;
        if(current_win_rate > max_win_rate) { // 选胜率最大的
            max_win_rate = current_win_rate;
            best_child = child;
            max_visits = child->visits;
        }
        else if(current_win_rate == max_win_rate && child->visits > max_visits) {
            max_visits = child->visits;
            best_child = child;
        } // 多个胜率最大的我们就选访问最多的
    }
    if (best_child) {
        // qDebug() << "visit:" << best_child->visits;
        // qDebug() << "winrate:" << max_win_rate;
        return best_child->move;
    } else { // 出bug了 瞎选
        std::vector<ChessMove> valid_moves;
        root->state.getValidMoves(valid_moves);
        if (!valid_moves.empty()) {
             std::uniform_int_distribution<> distrib(0, valid_moves.size() - 1);
             return valid_moves[distrib(randomEngine)];
        }
        return ChessMove();
    }
}