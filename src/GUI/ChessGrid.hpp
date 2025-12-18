#ifndef CHESSGRID_HPP
#define CHESSGRID_HPP

#include <QGraphicsRectItem>
#include <QColor>
#include <QString>

// 给state编码后我们就可以通过改变两个被点击的格子的样式来重绘

class ChessGrid : public QGraphicsRectItem
{
public:

    explicit ChessGrid(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr);

    // 设置格子的状态（空、黑棋、白棋、箭）
    void setState(QString newState);
    void setColor(QColor newColor);
    QString getState();

protected:
    // 重写 paint 方法来绘制格子和棋子
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QString state;// empty空格子 black黑棋 white白棋 arrow障碍
    // placePre放棋准备 亮黄色
    // arrowPre放箭准备 亮红色
    // TP是transparent缩写
    QColor gridColor;
};

#endif // CHESSGRID_HPP