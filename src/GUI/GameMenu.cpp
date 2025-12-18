#include "GameMenu.hpp"
#include "StyleSheets.hpp"
#include <QSlider>

GameMenu::GameMenu(QWidget *parent)
    : QWidget(parent)
{
    button1_0 = new QPushButton("创建新的人机对战游戏(人机先手)", this);
    button1_0->setGeometry(400, 200, 400, 50);
    button1_0->setStyleSheet(ButtonQSS::menuStyle);
    button1_0->setCursor(Qt::PointingHandCursor);

    button1_1 = new QPushButton("创建新的人机对战游戏(人机后手)", this);
    button1_1->setGeometry(400, 300, 400, 50);
    button1_1->setStyleSheet(ButtonQSS::menuStyle);
    button1_1->setCursor(Qt::PointingHandCursor);

    button2 = new QPushButton("创建新的双人游戏", this);
    button2->setGeometry(400, 400, 400, 50);
    button2->setStyleSheet(ButtonQSS::menuStyle);
    button2->setCursor(Qt::PointingHandCursor);
}

GameMenu::~GameMenu()
{

}