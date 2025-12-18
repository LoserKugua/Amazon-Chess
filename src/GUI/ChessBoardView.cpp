#include "ChessBoardView.hpp"
#include <QDebug>

ChessBoardView::~ChessBoardView() {

}

void ChessBoardView::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) { // 只处理左键点击
        QGraphicsView::mousePressEvent(event);
        return;
    }

    // ***将视图坐标转换为场景坐标
    QPointF scenePos = mapToScene(event->pos());
    qreal x = scenePos.x();
    qreal y = scenePos.y();

    if (x < 0 || x >= 640 || y < 0 || y >= 640) {// 越界
        QGraphicsView::mousePressEvent(event);
        return;
    }
    // 除一下算索引
    int col = static_cast<int>(x) / 80; // 列索引 = x坐标 / 格子宽度（80）
    int row = static_cast<int>(y) / 80; // 行索引 = y坐标 / 格子高度（80）

    emit clickedGrid(row, col);// 信号发送来
   
    QGraphicsView::mousePressEvent(event);
}