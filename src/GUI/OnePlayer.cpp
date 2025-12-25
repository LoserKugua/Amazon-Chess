#include "OnePlayer.hpp"
#include "../AI/MCTS.hpp"
#include <QMessageBox>
#include <QtConcurrent>
#include <QFutureWatcher>

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
        modeText->setText("AI思考中...");
        AImove();
        // 这里不要乱加东西。多线程会把主线程接着跑完。
    }
}

OnePlayer::~OnePlayer() {
}

 // MCTS占CPU导致ui无法刷新 只能上多线程
void OnePlayer::ApplyAImove(ChessMove bestMove) {
    // Engine_2->ChangePlayer(AIPlayWhich);
    Engine_2->HistoryPush(bestMove);
    if(AIPlayWhich == 0) {
        Engine_2->ModifyBoard(bestMove.GetFrom(), 0);
        Engine_2->ModifyBoard(bestMove.GetTo(), 1);
        Engine_2->ModifyBoard(bestMove.GetArrow(), 3);
        boardGrids[bestMove.GetFrom().x][bestMove.GetFrom().y]->setState("empty");
        boardGrids[bestMove.GetTo().x][bestMove.GetTo().y]->setState("black");
        boardGrids[bestMove.GetArrow().x][bestMove.GetArrow().y]->setState("arrow");
        Engine_2->ChangePlayer(1);
    }
    else {
        Engine_2->ModifyBoard(bestMove.GetFrom(), 0);
        Engine_2->ModifyBoard(bestMove.GetTo(), 2);
        Engine_2->ModifyBoard(bestMove.GetArrow(), 3);
        boardGrids[bestMove.GetFrom().x][bestMove.GetFrom().y]->setState("empty");
        boardGrids[bestMove.GetTo().x][bestMove.GetTo().y]->setState("white");
        boardGrids[bestMove.GetArrow().x][bestMove.GetArrow().y]->setState("arrow");
        Engine_2->ChangePlayer(0);
    }
    buttonSetable(1);
    gameOverJudge();
    ++gameTurns;
    Engine_2->modifyTurns(gameTurns);
    // qDebug() << "move turn:" << gameTurns;
    tipsMove.ChangeFrom((ChessPosition) {-1, -1});
    modeText->setText("人机对战模式");
}

void OnePlayer::AImove() {
    if(isGameOver) return;
    qDebug() << "test";
    modeText->setText("AI思考中...");
    buttonSetable(0);
    AIGameState nowState(Engine_2->GetState());
    int nowTurns = gameTurns;
    
    auto watcher = new QFutureWatcher<ChessMove>(this); // 进程监视器
    // 回调：当子线程运行结束，回调到主线程
    connect(watcher, &QFutureWatcher<ChessMove>::finished, this, [=]() {
        ChessMove AIbestmove = watcher->result();
        this->ApplyAImove(AIbestmove);
        watcher->deleteLater();
    });
    // MCTS会被扔到后台线程
    
    QFuture<ChessMove> future = QtConcurrent::run([nowState, nowTurns]() {
        // nowState.GetBoard().BoardDebug();
        MCTS mcts(nowState, 10000 + nowTurns * 100, 1000);
        return mcts.findBestMove();
    });
    watcher->setFuture(future); // watcher负责监视mcts算完没有
}

void OnePlayer::moreMove() {
    if(Engine_2->GetPlayer() == AIPlayWhich) {
        AImove();
    }
}

// 这里需要加一个判断，就是moreundo只会帮ai more undo
// 不然会出现最后一步是人类把自己下死了然后一直显示AI思考中
void OnePlayer::moreUndo() {
    if(gameStart()) {
        QMessageBox::warning(this, "提示", "当前没法再悔棋了!",
                                  QMessageBox::Ok);
        gameSaved = true;
        return;
    }
    if(Engine_2->GetPlayer() != AIPlayWhich) {
        // qDebug() << "no more undo";
        return;
    }
    ChessMove move = Engine_2->Undo();
    if(move.GetFrom().x == -1 && move.GetFrom().y == -1) {
        QMessageBox::warning(this, "提示", "当前没法再悔棋了!",
                                  QMessageBox::Ok);
        gameSaved = true;
        return; // 忘写return把程序卡崩了 6
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
    // qDebug() << "undo turn:" << gameTurns;
}

bool OnePlayer::gameStart() {
    if(AIPlayWhich == 0) return gameTurns == 1;
    return gameTurns == 0;
}