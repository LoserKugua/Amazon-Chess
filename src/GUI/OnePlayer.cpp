#include "OnePlayer.hpp"
#include "../AI/MCTS.hpp"
#include <QMessageBox>

OnePlayer::OnePlayer(QWidget *parent, bool AIPlayer)
    : TwoPlayer(parent) {
    AIPlayWhich = AIPlayer;
    if(AIPlayWhich == 1) {
        modeTextBlack = "人机对战模式";
        modeTextWhite = "AI思考中...";
        modeText->setText(modeTextBlack);
    }
    else {
        modeTextBlack = "AI思考中...";
        modeTextWhite = "人机对战模式";
        buttonSetable(0);
        AImove();
        buttonSetable(1);
        modeText->setText(modeTextWhite);
        Engine_2->ChangePlayer(1);
    }
}

OnePlayer::~OnePlayer() {
}

void OnePlayer::AImove() {
    if(isGameOver) {
        return;
    }
    modeText->setText("AI思考中...");
    buttonSetable(0);
    AIGameState nowState(Engine_2->GetState());
    MCTS mcts(nowState, 10000 + gameTurns * 100, 1000);
    ChessMove AIbestmove = mcts.findBestMove();
    Engine_2->HistoryPush(AIbestmove);
    if(AIPlayWhich == 0) {
        Engine_2->ModifyBoard(AIbestmove.GetFrom(), 0);
        Engine_2->ModifyBoard(AIbestmove.GetTo(), 1);
        Engine_2->ModifyBoard(AIbestmove.GetArrow(), 3);
        boardGrids[AIbestmove.GetFrom().x][AIbestmove.GetFrom().y]->setState("empty");
        boardGrids[AIbestmove.GetTo().x][AIbestmove.GetTo().y]->setState("black");
        boardGrids[AIbestmove.GetArrow().x][AIbestmove.GetArrow().y]->setState("arrow");
        Engine_2->ChangePlayer(1);
    }
    else {
        Engine_2->ModifyBoard(AIbestmove.GetFrom(), 0);
        Engine_2->ModifyBoard(AIbestmove.GetTo(), 2);
        Engine_2->ModifyBoard(AIbestmove.GetArrow(), 3);
        boardGrids[AIbestmove.GetFrom().x][AIbestmove.GetFrom().y]->setState("empty");
        boardGrids[AIbestmove.GetTo().x][AIbestmove.GetTo().y]->setState("white");
        boardGrids[AIbestmove.GetArrow().x][AIbestmove.GetArrow().y]->setState("arrow");
        Engine_2->ChangePlayer(0);
    }
    buttonSetable(1);
    tipsMove.ChangeFrom((ChessPosition) {-1, -1});
    ++gameTurns;
    Engine_2->modifyTurns(gameTurns);
    gameOverJudge();
    modeText->setText("人机对战模式");
}

void OnePlayer::moreMove() {
    if(Engine_2->GetPlayer() == AIPlayWhich) {
        AImove();
    }
}

void OnePlayer::moreUndo() {
    ChessMove move = Engine_2->Undo();
    if(move.GetFrom().x == -1 && move.GetFrom().y == -1) {
        QMessageBox::warning(this, "提示", "当前没法再悔棋了!",
                                  QMessageBox::Ok);
    }
    if(!Engine_2->GetPlayer()) {
        boardGrids[move.GetTo().x][move.GetTo().y]->setState("empty");
        boardGrids[move.GetArrow().x][move.GetArrow().y]->setState("empty");
        boardGrids[move.GetFrom().x][move.GetFrom().y]->setState("black");
        modeText->setText(modeTextBlack);
    }
    else {
        boardGrids[move.GetTo().x][move.GetTo().y]->setState("empty");
        boardGrids[move.GetArrow().x][move.GetArrow().y]->setState("empty");
        boardGrids[move.GetFrom().x][move.GetFrom().y]->setState("white");
        modeText->setText(modeTextWhite);
    }
    --gameTurns;
    Engine_2->modifyTurns(gameTurns);
}

bool OnePlayer::gameStart() {
    if(AIPlayWhich == 0) return gameTurns == 1;
    return gameTurns == 0;
}