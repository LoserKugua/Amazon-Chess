#include "GameEngine.hpp"
// 注意一下 botzone给的xy要swap一下
const GameState& GameEngine::GetState() {
    return CurrentState;
}

GameEngine::GameEngine() {

}

GameEngine::~GameEngine() {
    
}

bool GameEngine::GetPlayer() {// 0到黑1到白
    return CurrentState.GetPlayer();
}

void GameEngine::ChangePlayer(bool player) {
    CurrentState.ChangePlayer(player);
}

void GameEngine::ModifyBoard(ChessPosition pos, int value) {
    CurrentState.ModifyBoard(pos, value);
}

void GameEngine::HistoryPush(ChessMove move) {
    if(CurrentState.GetPlayer() == 0) {
        CurrentState.MoveBlack(move);
    }
    else {
        CurrentState.MoveWhite(move);
    }
}

void GameEngine::ModifyState(GameState state) {
    CurrentState = state;
}

int GameEngine::GenerateSave(QString filePath) {
    return CurrentState.GenerateSave(filePath);
}

int GameEngine::LoadSave(QString filePath) {
    return CurrentState.LoadSave(filePath);
}

void GameEngine::modifyTurns(int turns) {
    CurrentState.modifyTurns(turns);
}

int GameEngine::getTurns() {
    return CurrentState.getTurns();
}

ChessMove GameEngine::Undo() {
    return CurrentState.Undo();
}

bool GameEngine::ValidPos(const ChessPosition &pos) const {
    // 越界或被占了
    if(pos.x < 0 || pos.y < 0 || pos.x > 7 || pos.y > 7) return false;
    if(!CurrentState.GetBoard().IsAvailable(pos)) return false;
    return true;
}

std::vector<ChessPosition> GameEngine::GetValidArrows(const ChessPosition &pos) const {
    // 八个方向移动
    std::vector<ChessPosition> result;
    for(int i = 0; i <= 7; ++i) {
        int Tx = pos.x + CheckDx[i];
        int Ty = pos.y + CheckDy[i];
        if(ValidPos((ChessPosition) {Tx, Ty})) {
            result.push_back((ChessPosition) {Tx, Ty});
        }
    }
    return result;
}

std::vector<ChessPosition> GameEngine::GetValidMoves(const ChessPosition &pos) const {
    // 八个方向随机移动
    std::vector<ChessPosition> result;
    for(int i = 0; i <= 7; ++i) {
        int Tx = pos.x + CheckDx[i];
        int Ty = pos.y + CheckDy[i];
        while(ValidPos((ChessPosition) {Tx, Ty})) {
            result.push_back((ChessPosition) {Tx, Ty});
            Tx += CheckDx[i];
            Ty += CheckDy[i];
        }
    }
    return result;
}

int GameEngine::GameOver() {// 1黑赢2白赢
    const GameBoard& board = CurrentState.GetBoard();
    bool blackFlag = true;// 标记黑输了吗
    for(int i = 0; i <= 3; ++i) {
        std::vector<ChessPosition> Move_8_dir = GetValidArrows(board.GetBlackPos(i));
        if(Move_8_dir.size() != 0) {
            blackFlag = false;
            break;
        }
    }
    if(blackFlag) {
        CurrentState.GameOver();
        return 2;
    }
    bool whiteFlag = true;// 标记白输了吗
    for(int i = 0; i <= 3; ++i) {
        std::vector<ChessPosition> Move_8_dir = GetValidArrows(board.GetWhitePos(i));
        if(Move_8_dir.size() != 0) {
            whiteFlag = false;
            break;
        }
    }
    if(whiteFlag) {
        CurrentState.GameOver();
        return 1;
    }
    return 0;// 都没赢
}