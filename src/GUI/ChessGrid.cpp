#include "ChessGrid.hpp"

#include <QGraphicsScene>
#include <QGraphicsView>

ChessGrid::ChessGrid(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent)
    : QGraphicsRectItem(x, y, width, height, parent),
    state("empty")
{
    // 设置格子的默认边框
    setPen(QPen(Qt::black));
    // 背景颜色交给for了，就不搞了
}

void ChessGrid::setState(QString newState)
{
    if (state != newState) {
        state = newState;
        update(); // 状态改变时强制重绘
    }
    if(state == "empty" || state == "arrow") {
        setCursor(Qt::ArrowCursor);
    }
    else {
        setCursor(Qt::PointingHandCursor);
    }
}

void ChessGrid::setColor(QColor newColor) {
    gridColor = newColor;
}

QString ChessGrid::getState() {
    return state;
}

void ChessGrid::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen nPen;

    nPen.setColor(Qt::black);
    nPen.setWidth(3);
    nPen.setStyle(Qt::SolidLine);
    
    painter->setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter->setPen(nPen); // 使用已设置的边框
    painter->setBrush(gridColor);
    painter->drawRect(rect());

    setZValue(0);// 先默认下降回去
    // 根据状态绘制棋子
    // 因为有棋子格子绘制差不多，所以合并一下代码
    if (state == "black" || state == "blackChosen" || 
        state == "white" || state == "whiteChosen" || 
        state == "whiteTP" || state == "blackTP" ||
        state == "arrow" || state == "arrowTP" ||
        state == "arrowRed") {
        QRectF r = rect(); // 大小和位置
        qreal margin = r.width() * 0.1; // 边距
        QRectF pieceRect(r.x() + margin, r.y() + margin,
                 r.width() - 2 * margin, r.height() - 2 * margin);

        // 绘制阴影
        qreal shadowSpread = 0.03; // 阴影扩散程度(其实就缩放比例)
        qreal shadowOffset = 0.03; // 阴影偏移量
        QRectF shadowRect(pieceRect.x() + r.width() * shadowOffset,
                        pieceRect.y() + r.height() * shadowOffset,
                        pieceRect.width() * (1 + shadowSpread),
                        pieceRect.height() * (1 + shadowSpread));

        QColor shadowColor = Qt::black;
        shadowColor.setAlpha(100); // 设置透明度
        painter->setBrush(QBrush(shadowColor));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(shadowRect);

        // 主体
        if(state == "blackChosen" || state == "whiteChosen" || state == "arrowRed") {
            painter->setPen(QPen(Qt::red, 3));
        }
        else {
            painter->setPen(QPen(Qt::darkGray, 1));
        }

        QColor fillColor;
        if(state == "black" || state == "blackChosen" || state == "blackTP") fillColor = Qt::black;
        else if(state == "white" || state == "whiteChosen" || state == "whiteTP") fillColor = Qt::white;
        else fillColor = Qt::blue;
        if(state == "blackTP" || state == "whiteTP" || state == "arrowTP" || state == "arrowRed") fillColor.setAlpha(80);
        painter->setBrush(QBrush(fillColor));
        painter->drawEllipse(pieceRect);
    }
    else if (state == "placePre") {// 按PPT设计的
        QColor lightYellow(255, 255, 189);
        nPen.setColor(Qt::yellow);
        painter->setPen(nPen);
        painter->setBrush(lightYellow);
        painter->drawRect(rect());

        setZValue(1);// 图层浮上来一下不然渲染不好看
    }
    else if (state == "arrowPre") {
        QColor lightRed(255, 189, 189);
        nPen.setColor(Qt::red);
        painter->setPen(nPen);
        painter->setBrush(lightRed);
        painter->drawRect(rect());

        setZValue(1);
    }
}