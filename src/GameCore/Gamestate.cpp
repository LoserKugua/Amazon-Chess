#include "GameState.hpp"

GameState::GameState()
{
    CurrentPlayer = 0;
    GameOverTag = 0;
    gameTurns = 0;
}

void GameState::ChangePlayer(bool player) {
    // qDebug() << "currentplayer" << CurrentPlayer;
    CurrentPlayer = player;
    // qDebug() << "currentplayer" << CurrentPlayer;
}

bool GameState::GetPlayer() {
    return CurrentPlayer;
}

const GameBoard& GameState::GetBoard()  const {
    return Board;
}

void GameState::MoveBlack(ChessMove move) {
    Board.MoveBlack(move);
    HistoryMoveBlack.push_back(move);
}

void GameState::MoveWhite(ChessMove move) {
    Board.MoveWhite(move);
    HistoryMoveWhite.push_back(move);
}

void GameState::ModifyBoard(ChessPosition pos, int value) {
    Board.modify(pos, value);
}

void GameState::GameOver() {
    GameOverTag = true;
}

void GameState::modifyTurns(int turns) {
    gameTurns = turns;
}

int GameState::getTurns() {
    return gameTurns;
}

int GameState::GenerateSave(QString filePath) {
    QFile save(filePath);
    // 只写模式打开 保证清空原内容
    if(!save.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return 1;// 打不开文件
    }
    QTextStream qout(&save);
    qout << "Ha_Ji_Mi_O_Na_Mei_Lu_Duo\n";// 防伪标识 哈基米哦那没路躲
    qout << GameOverTag << '\n';// 先看完没完
    qout << CurrentPlayer << '\n';// 再看谁在玩
    qout << gameTurns << '\n';// 输出游戏轮数
    for(int i = 0; i <= 7; ++i) {
        for(int j = 0; j <= 7; ++j) {
            ChessPosition pos = (ChessPosition) {i, j};
            if(Board.IsBlack(pos)) qout << 1;
            else if(Board.IsWhite(pos)) qout << 2;
            else if(Board.IsArrow(pos)) qout << 3;
            else qout << 0;
            qout << ' ';
        }
        qout << '\n';
    }
    // 棋盘整完按编号输出棋子位置
    for(int i = 0; i <= 3; ++i) {
        qout << Board.GetBlackPos(i).x << ' ' << Board.GetBlackPos(i).y << '\n';
    }
    for(int i = 0; i <= 3; ++i) {
        qout << Board.GetWhitePos(i).x << ' ' << Board.GetWhitePos(i).y << '\n';
    }
    // 历史操作读取
    qout << HistoryMoveBlack.size() << '\n';
    for(size_t i = 0; i < HistoryMoveBlack.size(); ++i) {
        qout << HistoryMoveBlack[i].GetFrom().x << ' ';
        qout << HistoryMoveBlack[i].GetFrom().y << ' ';
        qout << HistoryMoveBlack[i].GetTo().x << ' ';
        qout << HistoryMoveBlack[i].GetTo().y << ' ';
        qout << HistoryMoveBlack[i].GetArrow().x << ' ';
        qout << HistoryMoveBlack[i].GetArrow().y << '\n';
    }
    qout << '\n';
    qout << HistoryMoveWhite.size() << '\n';
    for(size_t i = 0; i < HistoryMoveWhite.size(); ++i) {
        qout << HistoryMoveWhite[i].GetFrom().x << ' ';
        qout << HistoryMoveWhite[i].GetFrom().y << ' ';
        qout << HistoryMoveWhite[i].GetTo().x << ' ';
        qout << HistoryMoveWhite[i].GetTo().y << ' ';
        qout << HistoryMoveWhite[i].GetArrow().x << ' ';
        qout << HistoryMoveWhite[i].GetArrow().y << '\n';
    }
    qout << '\n';
    save.close();
    return 0;// 正常存档
}

bool posJudge(ChessPosition pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.x <= 7 && pos.y <= 7;
}

int GameState::LoadSave(QString filePath) {
    QFile save(filePath);
    if(!save.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 1;
    }
    QTextStream qin(&save);
    QString sign;
    qin >> sign;
    if(qin.status() != QTextStream::Ok) {// 没读到
        return 3;
    }
    if(sign != "Ha_Ji_Mi_O_Na_Mei_Lu_Duo") {// 不是存档类文件 校验不通过
        save.close();
        return 2;
    }
    // 读取结束标识和当前玩家
    int inputNum;
    qin >> inputNum;
    if(qin.status() != QTextStream::Ok) {// 读取错误
        return 3;
    }
    if(inputNum != 0 && inputNum != 1) {
        return 4;
    }
    else {
        GameOverTag = (bool) inputNum;
    }
    qin >> inputNum;
    if(qin.status() != QTextStream::Ok) {// 读取错误
        return 3;
    }
    if(inputNum != 0 && inputNum != 1) {
        return 4;
    }
    else {
        CurrentPlayer = (bool) inputNum;
    }
    qin >> inputNum;
    if(qin.status() != QTextStream::Ok) {// 读取错误
        return 3;
    }
    if(inputNum < 0) {
        return 4;
    }
    else {
        gameTurns = inputNum;
    }
    // 读取棋盘
    for(int i = 0; i <= 7; ++i) {
        for(int j = 0; j <= 7; ++j) {
            ChessPosition pos = (ChessPosition) {i, j};
            qin >> inputNum;
            if(qin.status() != QTextStream::Ok) {
                return 3;
            }
            if(inputNum != 0 && inputNum != 1 && inputNum != 2 && inputNum != 3) {
                return 4;
            }
            Board.modify(pos, inputNum);
        }
    }
    for(int i = 0; i <= 3; ++i) {
        ChessPosition pos;
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        Board.modifyBlack(pos, i);
    }
    for(int i = 0; i <= 3; ++i) {
        ChessPosition pos;
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        Board.modifyWhite(pos, i);
    }
    // 历史操作读取
    // 黑方
    HistoryMoveBlack.clear();
    HistoryMoveWhite.clear();
    int historyLength;
    qin >> historyLength;
    if(qin.status() != QTextStream::Ok) {
        return 3;
    }
    for (size_t i = 0; i < historyLength; i++) {
        ChessMove tempMove;
        ChessPosition pos;
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        tempMove.ChangeFrom(pos);
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        tempMove.ChangeTo(pos);
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        tempMove.ChangeArrow(pos);
        HistoryMoveBlack.push_back(tempMove);
    }
    // 白方历史操作
    qin >> historyLength;
    if(qin.status() != QTextStream::Ok) {
        return 3;
    }
    for (size_t i = 0; i < historyLength; i++) {
        ChessMove tempMove;
        ChessPosition pos;
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        tempMove.ChangeFrom(pos);
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        tempMove.ChangeTo(pos);
        qin >> pos.x >> pos.y;
        if(qin.status() != QTextStream::Ok) {
            return 3;
        }
        if(!posJudge(pos)) {
            return 4;
        }
        tempMove.ChangeArrow(pos);
        HistoryMoveWhite.push_back(tempMove);
    }
    // 判断对局是不是真正信息统一
    // 先是black/whieteChess统一不统一
    for(int i = 0; i <= 3; ++i) {
        if(!Board.IsBlack(Board.GetBlackPos(i))) {
            return 5;
        }
    }
    for(int i = 0; i <= 3; ++i) {
        if(!Board.IsWhite(Board.GetWhitePos(i))) {
            return 5;
        }
    }
    // 再是按历史记录下棋 看看得到的盘一不一样
    GameBoard board2;
    // 黑比白动的少 有问题
    if(HistoryMoveBlack.size() < HistoryMoveWhite.size()) return 6;
    for(size_t i = 0; i < HistoryMoveWhite.size(); ++i) {
        // 黑下一步
        ChessMove movei = HistoryMoveBlack[i];
        // 记录一下神经bug
        // 这里棋子把arrow放在自己之前的位置就会GG 棋盘更新没那么及时
        // 但是GUI是非常及时的 就。。。
        if(!board2.IsBlack(movei.GetFrom()) || !board2.IsAvailable(movei.GetTo())) {
            return 5;
        }// 这里判断的是过程中下棋有没有被阻挡 有没有移动空棋子
        if((movei.GetFrom() != movei.GetArrow()) && (!board2.IsAvailable(movei.GetArrow()))) {
            return 5;
        }
        board2.MoveBlack(movei);
        board2.modify(movei.GetFrom(), 0);
        board2.modify(movei.GetTo(), 1);
        board2.modify(movei.GetArrow(), 3);

        // board2.BoardDebug();
        // 白下一步
        movei = HistoryMoveWhite[i];
        if(!board2.IsWhite(movei.GetFrom()) || !board2.IsAvailable(movei.GetTo())) {
            return 5;
        }
        if((movei.GetFrom() != movei.GetArrow()) && (!board2.IsAvailable(movei.GetArrow()))) {
            return 5;
        }
        board2.MoveWhite(movei);
        board2.modify(movei.GetFrom(), 0);
        board2.modify(movei.GetTo(), 2);
        board2.modify(movei.GetArrow(), 3);

        // board2.BoardDebug();
    }
    if(HistoryMoveWhite.size() != HistoryMoveBlack.size()) {
        ChessMove movei = HistoryMoveBlack.back();
        if(!board2.IsBlack(movei.GetFrom()) || !board2.IsAvailable(movei.GetTo())) {
            return 5;
        }
        if((movei.GetFrom() != movei.GetArrow()) && (!board2.IsAvailable(movei.GetArrow()))) {
            return 5;
        }// 这里判断的是过程中下棋有没有被阻挡 有没有移动空棋子
        board2.MoveBlack(movei);
        board2.modify(movei.GetFrom(), 0);
        board2.modify(movei.GetTo(), 1);
        board2.modify(movei.GetArrow(), 3);
    }
    if(!(board2 == Board)) {
        // board2.BoardDebug();
        return 5;
    }
    save.close();
    return 0;
}
ChessMove GameState::Undo() {
    ChessMove move;
    if(HistoryMoveBlack.size() == 0 && HistoryMoveWhite.size() == 0) {
        move.ChangeFrom((ChessPosition) {-1, -1});
        return move;
    }
    if(CurrentPlayer == 1) {// 黑方
        move = HistoryMoveBlack.back();
        int index = Board.GetBlackIndex(move.GetTo());
        Board.modifyBlack(move.GetFrom(), index);
        Board.modify(move.GetTo(), 0);
        Board.modify(move.GetArrow(), 0);
        Board.modify(move.GetFrom(), 1); // 修BUG:Undo 也有顺序讲究
        HistoryMoveBlack.pop_back();
        CurrentPlayer = 0;
    }
    else {
        move = HistoryMoveWhite.back();
        int index = Board.GetWhiteIndex(move.GetTo());
        Board.modifyWhite(move.GetFrom(), index);
        Board.modify(move.GetTo(), 0);
        Board.modify(move.GetArrow(), 0);
        Board.modify(move.GetFrom(), 2);
        HistoryMoveWhite.pop_back();
        CurrentPlayer = 1;
    }
    return move;
}