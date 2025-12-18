#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QWidget>
#include <QKeyEvent>
#include <QPushButton>
#include <QSlider>
#include <QPixmap>
#include <QLabel>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>
#include <QCursor>
#include <vector>

class Settings : public QWidget {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent, QPixmap &s_pixmap);
    ~Settings();

signals:
    void exitSign();
    void bgUpdateSign();

private slots:
    void volumeChange(int value);
    void on_prevSongButton_clicked();
    void on_nextSongButton_clicked();
    void on_prevBgButton_clicked();
    void on_nextBgButton_clicked();

private:
    void keyPressEvent(QKeyEvent *event) override;
    void setupUI();
    void setupMusicUI();
    void setupBgUI();

    QLabel *titleLabel;
    QLabel *setBackground;
    
    // 放歌相关
    QMediaPlayer *setPlayer;
    QAudioOutput *setAOutput;
    // 切歌的
    QPushButton *prevSongButton;
    QLabel *prevSongLabel;
    QPushButton *nextSongButton;
    QLabel *nextSongLabel;
    QLabel *currentSongLabel;
    std::vector<QString> songNames;
    std::vector<QString> songUrls;
    int currentSongIndex;
    // 调音量的
    QSlider *volumeSlider;
    QLabel *volumeLabel;    
    // 切背景的
    QPixmap &setPixmap;
    QPushButton *prevBgButton;
    QLabel *prevBgLabel;
    QPushButton *nextBgButton;
    QLabel *nextBgLabel;
    QLabel *currentBgLabel;
    std::vector<QString> bgNames;
    std::vector<QString> bgUrls;
    int currentBgIndex;
    // 布局管理的
    QStackedWidget *setStack;
    QWidget *musicSettings;
    QWidget *bgSettings;
    QPushButton *musicOptions; // 这俩按钮用于左侧导航栏
    QPushButton *bgOptions;
    QPushButton *lastOptionsButton; // 体现了代码的可扩展性()
    // 退出的
    QLabel *exitLabel;
};

#endif