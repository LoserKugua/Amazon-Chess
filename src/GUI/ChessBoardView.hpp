// 这个文件算是真的石山代码堆叠了没办法
// 我要获取点击坐标所以得重写一下QGraphicView
#ifndef CHESSBOARDVIEW_HPP
#define CHESSBOARDVIEW_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

class ChessBoardView : public QGraphicsView {
    Q_OBJECT

signals:
    void clickedGrid(int col, int row); // 发射信号用于模式里游戏运作，点到哪个格子了

protected:
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit ChessBoardView(QGraphicsScene *scene, QWidget *parent = nullptr) 
        : QGraphicsView(scene, parent) {}
    ~ChessBoardView();
};

#endif