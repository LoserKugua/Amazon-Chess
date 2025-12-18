#ifndef ONEPLAYER_HPP
#define ONEPLAYER_HPP

#include "TwoPlayer.hpp"

class OnePlayer : public TwoPlayer {
    Q_OBJECT

private:
    bool AIPlayWhich; // 0代表AI执棋黑方 1代表AI执棋白方
    void AImove();
    void moreMove() override;
    void moreUndo() override;
    bool gameStart() override;

public:
    explicit OnePlayer(QWidget *parent = nullptr, bool AIPlayer = false);
    virtual ~OnePlayer();
};

#endif