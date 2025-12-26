#include "TwoPlayer.hpp"
#include "StyleSheets.hpp"
#include "../AI/MCTS.hpp"
#include <QIcon>
#include <QMessageBox>

/*
To-do list:
6.算法优化
*/

TwoPlayer::TwoPlayer(QWidget *parent)
    : ChessBoard(parent)
{// 整局游戏初始化
    modeTextBlack = "双人模式 当前轮到：黑方";
    modeTextWhite = "双人模式 当前轮到：白方";
    modeTextEnd = "游戏已结束";
    modeText = new QLabel(modeTextBlack, this);
    modeText->setGeometry(80, 0, 400, 80);
    modeText->setStyleSheet("QLabel{"
                            "font-size:25px;"
                            "color:black;"
                            "background-color:lightblue;"
                            "}");
    modeText->show();
    connect(view, &ChessBoardView::clickedGrid, this, &TwoPlayer::boardClickResponse);

    Engine_2 = new GameEngine;
    setupChessBoard(Engine_2->GetState().GetBoard());
    chosenChess = (ChessPosition) {-1, -1};
    placeStage = 0; // 黑方先手
    isGameOver = false;
    isTiping = false;
    gameSaved = true;
    gameTurns = 0;
    tipsMove.ChangeFrom((ChessPosition) {-1, -1});
    buttonsInit();
    connect(this->saveButton, &QPushButton::clicked, this, &TwoPlayer::on_saveButton_clicked);
    connect(this->loadButton, &QPushButton::clicked, this, &TwoPlayer::on_loadButton_clicked);
    connect(this->undoButton, &QPushButton::clicked, this, &TwoPlayer::on_undoButton_clicked);
    connect(this->tipsButton, &QPushButton::clicked, this, &TwoPlayer::on_tipsButton_clicked);
}

TwoPlayer::~TwoPlayer() {
}

bool TwoPlayer::isGameSaved() {
    return gameSaved;
}

void TwoPlayer::buttonsInit() {// 所有button初始化
    // 退出按钮
    QIcon exitIcon(":/images/exit.png");
    exitButton = new QPushButton(exitIcon, "退出", this);
    exitButton->setStyleSheet(ButtonQSS::exitStyle);
    ButtonQSS::shadowInit(exitButton);// 阴影效果
    exitButton->setIconSize(QSize(35, 35));
    exitButton->move(850, 600);
    exitButton->show();
    exitButton->setCursor(Qt::PointingHandCursor);
    // 存档按钮
    QIcon saveIcon(":/images/save.png");
    saveButton = new QPushButton(saveIcon, "存档", this);
    saveButton->setStyleSheet(ButtonQSS::saveStyle);
    ButtonQSS::shadowInit(saveButton);
    saveButton->setIconSize(QSize(35, 35));
    saveButton->move(850, 500);
    saveButton->show();
    saveButton->setCursor(Qt::PointingHandCursor);
    // 读档按钮
    QIcon loadIcon(":/images/load.png");
    loadButton = new QPushButton(loadIcon, "读档", this);
    loadButton->setStyleSheet(ButtonQSS::loadStyle);
    ButtonQSS::shadowInit(loadButton);
    loadButton->setIconSize(QSize(35, 35));
    loadButton->move(850, 400);
    loadButton->show();
    loadButton->setCursor(Qt::PointingHandCursor);
    // 悔棋按钮
    QIcon undoIcon(":/images/undo.png");
    undoButton = new QPushButton(undoIcon, "悔棋", this);
    undoButton->setStyleSheet(ButtonQSS::undoStyle);
    ButtonQSS::shadowInit(undoButton);
    undoButton->setIconSize(QSize(35, 35));
    undoButton->move(850, 300);
    undoButton->show();
    undoButton->setCursor(Qt::PointingHandCursor);
    // 提示按钮
    QIcon tipsIcon(":/images/tips.png");
    tipsButton = new QPushButton(tipsIcon, "提示", this);
    tipsButton->setStyleSheet(ButtonQSS::tipsStyle);
    ButtonQSS::shadowInit(tipsButton);
    tipsButton->setIconSize(QSize(35, 35));
    tipsButton->move(850, 200);
    tipsButton->show();
    tipsButton->setCursor(Qt::PointingHandCursor);
    // 设置按钮
    QIcon settingsIcon(":/images/settings.png");
    settingsButton = new QPushButton(settingsIcon, "设置", this);
    settingsButton->setStyleSheet(ButtonQSS::settingsStyle);
    ButtonQSS::shadowInit(settingsButton);
    settingsButton->setIconSize(QSize(35, 35));
    settingsButton->move(850, 100);
    settingsButton->show();
    settingsButton->setCursor(Qt::PointingHandCursor);
}

void TwoPlayer::buttonSetable(bool mode) {
    if(mode == false) {
        exitButton->setDisabled(true);
        saveButton->setDisabled(true);
        loadButton->setDisabled(true);
        tipsButton->setDisabled(true);
        undoButton->setDisabled(true);
        settingsButton->setDisabled(true);
    }
    else {
        exitButton->setEnabled(true);
        saveButton->setEnabled(true);
        loadButton->setEnabled(true);
        tipsButton->setEnabled(true);
        undoButton->setEnabled(true);
        settingsButton->setEnabled(true);
    }
}

void TwoPlayer::finishMoveMsg() {
    QMessageBox::information(this, "提示", "请先把这一步棋走完(放完箭之后)", QMessageBox::Ok);
}

int TwoPlayer::saveMessage() {// 1是确定不存档 0是再想想
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "存档提示", "您的本局游戏还未存档，您确定要这么做吗?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) return 1;
    return 0;
}

int TwoPlayer::suffixMessage() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "文件打开提示", "您打开的不是指定类型(.txt)的文件，您确定要这么做吗?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) return 1;
    return 0;
}

// 进行一个存档读取or写出的注释:
// 0表示no problem
// 1表示文件打不开
// 2表示不是存档类文件
// 3表示读取过程中没有输入成功
// 4表示解析数字不在范围之类的(比如棋盘来个4 坐标来个9)
// 5表示内部信息不统一

void TwoPlayer::on_saveButton_clicked() {
    // 选文件gui
    if(placeStage != 0) {
        finishMoveMsg();
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "游戏存档",
        QDir::currentPath() + "/untitled.txt", // 默认文件名和路径
        "文本文件 (*.txt);;所有文件 (*)"
    );
    if(!filePath.isEmpty()) {
        Engine_2->GenerateSave(filePath);
    }
    else {
        QMessageBox::warning(this, "文件保存出错", "文件保存路径无法被找到或创建",
                                  QMessageBox::Ok);
        return;
    }
    gameSaved = true;
}

void TwoPlayer::on_loadButton_clicked() {
    if(placeStage != 0) {
        finishMoveMsg();
        return;
    }
    if(!gameSaved) {
        int flag = saveMessage();
        if(!flag) return;
    }
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "游戏读档",
        QDir::currentPath() + "/untitled.txt", // 默认文件名和路径
        "文本文件 (*.txt);;所有文件 (*)"
    );
    QFileInfo fileInfo(filePath);
    if(fileInfo.suffix() != "txt") {
        if(!suffixMessage()) {
            return;
        }
    }
    GameState tempState;
    int flag = tempState.LoadSave(filePath);
    if(flag == 0) {
        Engine_2->ModifyState(tempState);
        gameSaved = true;
        isTiping = false;
        gameTurns = Engine_2->getTurns();
        tipsMove.ChangeFrom((ChessPosition) {-1, -1});
        isGameOver = Engine_2->GameOver();
        reRender();
        moreMove();
        // qDebug() << "turn:" << Engine_2->getTurns() << '\n';
        // 在这里这个函数什么都不干
        // 交给oneplayer重写的话就不用扯一堆了
    }
    else if(flag == 1) {
        QMessageBox::warning(this, "文件寻找出错", "文件无法被找到",
                                  QMessageBox::Ok);
    }
    else if(flag == 2) {
        QMessageBox::warning(this, "文件类型出错", "这不是一个Amazon Chess游戏的存档文件",
                                  QMessageBox::Ok);
    }
    else if(flag == 3) {
        QMessageBox::warning(this, "文件读取出错", "文件无法正常读取",
                                  QMessageBox::Ok);
    }
    else if(flag == 4) {
        QMessageBox::warning(this, "文件解析出错", "文件内保存的棋盘信息数据不在合规范围",
                                  QMessageBox::Ok);
    }
    else if(flag == 5) {
        QMessageBox::warning(this, "文件解析出错", "文件内保存的棋盘信息不统一，不是一个正常的棋局",
                                  QMessageBox::Ok);
    }
    else if(flag == 6) {
        QMessageBox::warning(this, "文件解析出错", "文件内保存的棋盘黑方不是先手",
                                  QMessageBox::Ok);
    }
}

// 需要注意的是 engine里已经把player换过来了
void TwoPlayer::on_undoButton_clicked() {
    if(placeStage != 0) {
        finishMoveMsg();
        return;
    }
    if(gameStart()) {
        QMessageBox::warning(this, "提示", "当前没法再悔棋了!",
                                  QMessageBox::Ok);
        gameSaved = true;
        return;
    }
    tipsMove.ChangeFrom((ChessPosition) {-1, -1});
    ChessMove move = Engine_2->Undo();
    if(move.GetFrom().x == -1 && move.GetFrom().y == -1) {
        QMessageBox::warning(this, "提示", "当前没法再悔棋了!",
                                  QMessageBox::Ok);
        gameSaved = true;
    }
    else {
        isGameOver = false;
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
        moreUndo();
    }
}

void TwoPlayer::on_tipsButton_clicked() {
    if(placeStage != 0) {
        finishMoveMsg();
        return;
    }
    if(isGameOver == true) {
        QMessageBox::information(this, "游戏已结束", "当前无法使用提示哦", QMessageBox::Ok);
        return;
    }
    if(tipsMove.GetFrom().x == -1) {
        AIGameState nowState(Engine_2->GetState());
        MCTS mcts(nowState, 1000LL);
        tipsMove = mcts.findBestMove();
    }
    if(boardGrids[tipsMove.GetFrom().x][tipsMove.GetFrom().y]->getState() == "black") {
        boardGrids[tipsMove.GetTo().x][tipsMove.GetTo().y]->setState("blackTP");
        boardGrids[tipsMove.GetFrom().x][tipsMove.GetFrom().y]->setState("blackChosen");
    }
    else {
        boardGrids[tipsMove.GetTo().x][tipsMove.GetTo().y]->setState("whiteTP");
        boardGrids[tipsMove.GetFrom().x][tipsMove.GetFrom().y]->setState("whiteChosen");
    }
    boardGrids[tipsMove.GetArrow().x][tipsMove.GetArrow().y]->setState("arrowTP");
    if(tipsMove.GetFrom() == tipsMove.GetArrow()) {
        boardGrids[tipsMove.GetArrow().x][tipsMove.GetArrow().y]->setState("arrowRed");
    }
    buttonSetable(false);
    isTiping = true;
    QMessageBox::information(this, "提示", "点击棋盘退出提示", QMessageBox::Ok);
}

void TwoPlayer::gameOverJudge() {
    int flag = Engine_2->GameOver();
    if(flag == 0) return;
    modeText->setText(modeTextEnd);
    if(flag == 1) {
        QMessageBox::information(this, "游戏结束", "恭喜黑方获胜！", QMessageBox::Ok);
        isGameOver = true;
    }
    else if(flag == 2) {
        QMessageBox::information(this, "游戏结束", "恭喜白方获胜！", QMessageBox::Ok);
        isGameOver = true;
    }
}

void TwoPlayer::placeRender(int renderMode) {
    if(renderMode == 0) {
        for(auto posi : placePres) {
            if(boardGrids[posi.x][posi.y]->getState() == "arrow")
                continue;
            boardGrids[posi.x][posi.y]->setState("placePre");
        }
    }
    else if(renderMode == 1) {
        for(auto posi : placePres) {
            if(boardGrids[posi.x][posi.y]->getState() == "arrow")
                continue;
            boardGrids[posi.x][posi.y]->setState("arrowPre");
        }
    }
    else if(renderMode == 2) {
        for(auto posi : placePres) {
            if(boardGrids[posi.x][posi.y]->getState() == "arrow")
                continue;
            boardGrids[posi.x][posi.y]->setState("empty");
        }
    }
}

void TwoPlayer::reRender() {
    for(int i = 0; i <= 7; ++i) {
        for(int j = 0; j <= 7; ++j) {
            ChessPosition pos = {i, j};
            if(Engine_2->GetState().GetBoard().IsAvailable(pos)) {
                boardGrids[i][j]->setState("empty");
            }
            else if(Engine_2->GetState().GetBoard().IsBlack(pos)) {
                boardGrids[i][j]->setState("black");
            }
            else if(Engine_2->GetState().GetBoard().IsWhite(pos)) {
                boardGrids[i][j]->setState("white");
            }
            else if(Engine_2->GetState().GetBoard().IsArrow(pos)) {
                boardGrids[i][j]->setState("arrow");
            }
        }
    }
    if(isGameOver) {
        modeText->setText(modeTextEnd);
    }
    else if(!Engine_2->GetPlayer()) {
        modeText->setText(modeTextBlack);
    }
    else {
        modeText->setText(modeTextWhite);
    }
}

void TwoPlayer::chessChoosing(int col, int row) {
    if(Engine_2->GetPlayer() == 0) {
        if(boardGrids[col][row]->getState() == "black") {
            boardGrids[col][row]->setState("blackChosen");
            if(chosenChess.x != -1) {// 只能选一个棋子 互斥机制
                boardGrids[chosenChess.x][chosenChess.y]->setState("black");
                placeRender(2);// 原本的取消了
            }
            chosenChess = (ChessPosition) {col, row};
            placePres = Engine_2->GetValidMoves(chosenChess);
            placeRender(0);// 渲染placepre
        }
        else if(boardGrids[col][row]->getState() == "blackChosen") {
            boardGrids[col][row]->setState("black");
            placeRender(2);// 原本的取消了
            chosenChess = (ChessPosition) {-1, -1};// 没有被选的
        }
    }
    else {
        if(boardGrids[col][row]->getState() == "white") {
            boardGrids[col][row]->setState("whiteChosen");
            if(chosenChess.x != -1) {// 只能选一个棋子 互斥机制
                boardGrids[chosenChess.x][chosenChess.y]->setState("white");
                placeRender(2);// 原本的取消了
            }
            chosenChess = (ChessPosition) {col, row};
            placePres = Engine_2->GetValidMoves(chosenChess);
            placeRender(0);// 渲染placepre
        }
        else if(boardGrids[col][row]->getState() == "whiteChosen") {
            boardGrids[col][row]->setState("white");
            placeRender(2);// 原本的取消了
            chosenChess = (ChessPosition) {-1, -1};// 没有被选的
        }
    }
    if(chosenChess.x == -1) placeStage = 0;
    else placeStage = 1;
}

void TwoPlayer::chessPlacing(int col, int row) {
    if(boardGrids[col][row]->getState() == "placePre") {
        placeRender(2);// 渲染掉placePre
        
        Engine_2->ModifyBoard(chosenChess, 0);
        Engine_2->ModifyBoard((ChessPosition) {col, row}, 1 +(int)Engine_2->GetPlayer());

        currentMove.ChangeFrom(chosenChess);
        currentMove.ChangeTo((ChessPosition) {col, row});

        QString type = boardGrids[chosenChess.x][chosenChess.y]->getState();// 渲染棋子移动
        boardGrids[chosenChess.x][chosenChess.y]->setState("empty");
        if(type == "blackChosen") {
            boardGrids[col][row]->setState("black");
        }
        else {
            boardGrids[col][row]->setState("white");
        }
        
        placePres = Engine_2->GetValidMoves((ChessPosition) {col, row});
        placeRender(1);// 渲染arrowPre

        chosenChess = (ChessPosition) {col, row};
        placeStage = 2;

        gameSaved = false;
    }
}

void TwoPlayer::arrowPlacing(int col, int row) {
    if(boardGrids[col][row]->getState() == "arrowPre") {
        placeRender(2);// 渲染掉arrowPre

        currentMove.ChangeArrow((ChessPosition) {col, row});
        // currentMove.MoveDebug();
        Engine_2->HistoryPush(currentMove);
        Engine_2->ModifyBoard((ChessPosition) {col, row}, 3);

        if(Engine_2->GetPlayer() == 0) {
            Engine_2->ChangePlayer(1);// ***换玩家
            modeText->setText(modeTextWhite);
        }
        else {
            Engine_2->ChangePlayer(0);// ***换玩家
            modeText->setText(modeTextBlack);
        }

        boardGrids[col][row]->setState("arrow");

        chosenChess = (ChessPosition) {-1, -1};
        placeStage = 0;
        // Engine_2.GetState().GetBoard().BlackChessDebug();
        // Engine_2.GetState().GetBoard().WhiteChessDebug();

        gameSaved = false;
        ++gameTurns;
        Engine_2->modifyTurns(gameTurns);
        // qDebug() << "move turn:" << gameTurns;
    }
}

void TwoPlayer::boardClickResponse(int col, int row) {
    if(isGameOver) return;
    if(isTiping) {
        isTiping = false;
        buttonSetable(true);
        boardGrids[tipsMove.GetArrow().x][tipsMove.GetArrow().y]->setState("empty");
        if(boardGrids[tipsMove.GetTo().x][tipsMove.GetTo().y]->getState() == "blackTP") {
            boardGrids[tipsMove.GetFrom().x][tipsMove.GetFrom().y]->setState("black");
        }
        else {
            boardGrids[tipsMove.GetFrom().x][tipsMove.GetFrom().y]->setState("white");
        }
        boardGrids[tipsMove.GetTo().x][tipsMove.GetTo().y]->setState("empty");
        return;
    }
    // 棋子的选择与取消选择
    if(placeStage == 0) {
        chessChoosing(col, row);
    }
    else if(placeStage == 1) {
        chessChoosing(col, row);
        chessPlacing(col, row);
    }
    else if(placeStage == 2) {
        arrowPlacing(col, row);
        gameOverJudge();
        // 一定不要在这里改gameturns。
        // 不然鼠标点两下就加了。
        moreMove();
        tipsMove.ChangeFrom((ChessPosition) {-1, -1});
    }
    // boardGrids[col][row]->update();
}

bool TwoPlayer::gameStart() {
    return gameTurns == 0;
}

void TwoPlayer::moreMove() { // do nothing
}

void TwoPlayer::moreUndo() { // do nothing

}