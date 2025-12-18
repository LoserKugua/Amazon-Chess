#ifndef GAMEMENU_HPP
#define GAMEMENU_HPP

#include <QWidget>
#include <QPushButton>

class GameMenu : public QWidget
{
    Q_OBJECT

private:
    
public:
    QPushButton *button1_0; // 单人模式 人机先手
    QPushButton *button1_1; // 单人模式 人机后手
    QPushButton *button2; // 双人模式
    explicit GameMenu(QWidget *parent = nullptr);
    ~GameMenu();
};

#endif