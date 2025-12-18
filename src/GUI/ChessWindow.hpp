#ifndef CHESSWINDOW_HPP
#define CHESSWINDOW_HPP

#include <QMainWindow>
#include <QStackedWidget>
#include "GameMenu.hpp"
#include "TwoPlayer.hpp"
#include "OnePlayer.hpp"
#include "Settings.hpp"

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChessWindow(QWidget *parent = nullptr);
    ~ChessWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    GameMenu *menu;
    OnePlayer *windowBoard1;
    TwoPlayer *windowBoard2;
    QStackedWidget *mainStack;
    Settings *settingsWidget;
    void transTwoPlayer();
    void transOnePlayer_0();
    void transOnePlayer_1();
    void transMenu1();
    void transMenu2();

    QPixmap backgroundPixmap;
    void updateBackground();
};

#endif // CHESSWINDOW_HPP