#include "ChessWindow.hpp"
#include <QDebug>
#include <QVBoxLayout>

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 窗口菜单初始化
    resize(1204, 790);
    setWindowTitle("Amazon Chess");
    menu = new GameMenu(this);
    mainStack = new QStackedWidget(this);
    mainStack->addWidget(menu);
    setCentralWidget(mainStack);
    mainStack->setCurrentIndex(0);
    connect(menu->button1_0, &QPushButton::clicked, this, &ChessWindow::transOnePlayer_0);
    connect(menu->button1_1, &QPushButton::clicked, this, &ChessWindow::transOnePlayer_1);
    connect(menu->button2, &QPushButton::clicked, this, &ChessWindow::transTwoPlayer);
    // 加载背景图
    backgroundPixmap.load(":/images/background1.jpg");
    updateBackground();
    windowBoard1 = new OnePlayer(this);
    windowBoard2 = new TwoPlayer(this);
    mainStack->addWidget(windowBoard1);
    mainStack->addWidget(windowBoard2);
    settingsWidget = new Settings(this, backgroundPixmap);
    mainStack->addWidget(settingsWidget);
    connect(settingsWidget, &Settings::bgUpdateSign, this, &ChessWindow::updateBackground);
}

ChessWindow::~ChessWindow() {
}

void ChessWindow::transOnePlayer_0() {
    // 新建游戏
    mainStack->removeWidget(windowBoard1);
    delete windowBoard1;
    windowBoard1 = new OnePlayer(this, false);
    mainStack->insertWidget(1, windowBoard1);
    // 连接信号 退出
    connect(windowBoard1->exitButton, &QPushButton::clicked, this, &ChessWindow::transMenu1);
    connect(windowBoard1->settingsButton, &QPushButton::clicked, this, [this]{
        mainStack->setCurrentIndex(3);
    });
    connect(settingsWidget, &Settings::exitSign, this, [this]{
        mainStack->setCurrentIndex(1);
    });
    mainStack->setCurrentIndex(1);
}

void ChessWindow::transOnePlayer_1() {
    // 新建游戏
    mainStack->removeWidget(windowBoard1);
    delete windowBoard1;
    windowBoard1 = new OnePlayer(this, true);
    mainStack->insertWidget(1, windowBoard1);
    // 连接信号 退出
    connect(windowBoard1->exitButton, &QPushButton::clicked, this, &ChessWindow::transMenu1);
    connect(windowBoard1->settingsButton, &QPushButton::clicked, this, [this]{
        mainStack->setCurrentIndex(3);
    });
    connect(settingsWidget, &Settings::exitSign, this, [this]{
        mainStack->setCurrentIndex(1);
    });
    mainStack->setCurrentIndex(1);
}

void ChessWindow::transTwoPlayer() {
    // 新建游戏
    mainStack->removeWidget(windowBoard2);
    delete windowBoard2;
    windowBoard2 = new TwoPlayer(this);
    mainStack->insertWidget(2, windowBoard2);
    // 连接信号 退出
    connect(windowBoard2->exitButton, &QPushButton::clicked, this, &ChessWindow::transMenu2);
    connect(windowBoard2->settingsButton, &QPushButton::clicked, this, [this]{
        mainStack->setCurrentIndex(3);
    });
    connect(settingsWidget, &Settings::exitSign, this, [this]{
        mainStack->setCurrentIndex(2);
    });
    mainStack->setCurrentIndex(2);
}

void ChessWindow::transMenu1() {
    if(!windowBoard1->isGameSaved()) {
        if(!windowBoard1->saveMessage()) {
            return;
        }
    }
    mainStack->setCurrentIndex(0);
}


void ChessWindow::transMenu2() {
    if(!windowBoard2->isGameSaved()) {
        if(!windowBoard2->saveMessage()) {
            return;
        }
    }
    mainStack->setCurrentIndex(0);
}

// 重写缩放事件 带着缩放图片
void ChessWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    updateBackground();
}

void ChessWindow::updateBackground() {
    QPalette palette = this->palette(); // 获取当前窗口的调色板
    QPixmap scaledPixmap = backgroundPixmap.scaled(
        this->size(),
        Qt::IgnoreAspectRatio,// 忽略原始图片比例
        Qt::SmoothTransformation// 平滑缩放
    );
    palette.setBrush(QPalette::Window, QBrush(scaledPixmap)); // 用缩放后的图片设置背景
    setPalette(palette); // 应用新的调色板
}