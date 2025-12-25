#include "Settings.hpp"
#include "StyleSheets.hpp"

Settings::Settings(QWidget *parent, QPixmap &s_pixmap)
    : QWidget(parent) 
    , setPixmap(s_pixmap) {
    setupUI();
}

Settings::~Settings() {

}

void Settings::setupUI() {
    // 主标题
    titleLabel = new QLabel("设置", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->move(560, 10);
    titleLabel->setStyleSheet(ButtonQSS::titleLabelStyle);

    exitLabel = new QLabel("(按Esc退出)", this);
    exitLabel->setAlignment(Qt::AlignCenter);
    exitLabel->move(640, 10);
    exitLabel->setStyleSheet(ButtonQSS::titleLabelStyle);

    setBackground = new QLabel(this);
    setBackground->setGeometry(50, 50, 1100, 700);
    setBackground->setStyleSheet("background-color: #80000000; border: 1px solid black;");
    
    musicOptions = new QPushButton("音乐选项", this);
    musicOptions->setGeometry(50, 50, 150, 50);
    musicOptions->setStyleSheet(ButtonQSS::defaultButton);
    musicOptions->setCursor(Qt::PointingHandCursor);
    connect(musicOptions, &QPushButton::clicked, this, [this] {
        lastOptionsButton->setEnabled(true);
        musicOptions->setDisabled(true);
        setStack->setCurrentIndex(0);
        lastOptionsButton = musicOptions;
    });
    musicOptions->setDisabled(true);
    lastOptionsButton = musicOptions;

    bgOptions = new QPushButton("背景选项", this);
    bgOptions->setGeometry(50, 100, 150, 50);
    bgOptions->setStyleSheet(ButtonQSS::defaultButton);
    bgOptions->setCursor(Qt::PointingHandCursor);
    connect(bgOptions, &QPushButton::clicked, this, [this] {
        lastOptionsButton->setEnabled(true);
        bgOptions->setDisabled(true);
        setStack->setCurrentIndex(1);
        lastOptionsButton = bgOptions;
    });
    
    // setupMusicUI();
    musicSettings = new QWidget(this);
    setupBgUI();

    setStack = new QStackedWidget(this);
    setStack->addWidget(musicSettings);
    setStack->addWidget(bgSettings);
    setStack->setGeometry(200, 50, 950, 700);
    setStack->setStyleSheet("background-color: rgba(128, 128, 128, 160)");
    setStack->setCurrentIndex(0);
    setStack->show();

}

void Settings::setupMusicUI() { // 切歌ui实现
    musicSettings = new QWidget(this);
    prevSongButton = new QPushButton("◀", musicSettings);
    prevSongButton->setCursor(Qt::PointingHandCursor);
    prevSongButton->setStyleSheet(ButtonQSS::arrowButton);
    prevSongButton->move(250, 50);
    nextSongButton = new QPushButton("▶", musicSettings);
    nextSongButton->setCursor(Qt::PointingHandCursor);
    nextSongButton->setStyleSheet(ButtonQSS::arrowButton);
    nextSongButton->move(650, 50);
    currentSongLabel = new QLabel("A Glimpse of Tomorrow", musicSettings);
    currentSongLabel->setAlignment(Qt::AlignCenter);
    currentSongLabel->setStyleSheet(ButtonQSS::labelStyle);
    currentSongLabel->setGeometry(280, 50, 370, 50);
    prevSongLabel = new QLabel("(上一首)", musicSettings);
    prevSongLabel->setAlignment(Qt::AlignCenter);
    prevSongLabel->setStyleSheet(ButtonQSS::labelStyle2);
    prevSongLabel->setGeometry(150, 50, 100, 50);
    nextSongLabel = new QLabel("(下一首)", musicSettings);
    nextSongLabel->setAlignment(Qt::AlignCenter);
    nextSongLabel->setStyleSheet(ButtonQSS::labelStyle2);
    nextSongLabel->setGeometry(700, 50, 100, 50);
    connect(prevSongButton, &QPushButton::clicked, this, &on_prevSongButton_clicked);
    connect(nextSongButton, &QPushButton::clicked, this, &on_nextSongButton_clicked);
    // 切歌核心实现
    songNames.push_back("A Glimpse of Tomorrow");
    songNames.push_back("A Glimpse of Tomorrow(2)");
    songNames.push_back("Bittersweet Echoes");
    songNames.push_back("Bittersweet Echoes(2)");
    
    songUrls.push_back("qrc:/music/AGlimpseofTomorrow.mp3");
    songUrls.push_back("qrc:/music/AGlimpseofTomorrow_2.mp3");
    songUrls.push_back("qrc:/music/BittersweetEchoes.mp3");
    songUrls.push_back("qrc:/music/BittersweetEchoes_2.mp3");

    currentSongIndex = 0;
    setPlayer = new QMediaPlayer(this);
    setAOutput = new QAudioOutput(this);
    setPlayer->setAudioOutput(setAOutput);
    setPlayer->setSource(QUrl(songUrls[currentSongIndex]));
    setPlayer->play();
    // 检测到播放完毕切歌
    connect(setPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            on_nextSongButton_clicked();
            // qDebug() << "切歌";
        }
    });

    volumeSlider = new QSlider(Qt::Horizontal, musicSettings);
    volumeSlider->move(100, 130);
    volumeSlider->setStyleSheet(ButtonQSS::volumeSlider);
    volumeSlider->setFixedWidth(600);
    volumeSlider->setCursor(Qt::PointingHandCursor);
    volumeSlider->setValue(100);
    volumeSlider->setMaximum(100); // 默认最大值99 嚯嚯夸张哦
    connect(volumeSlider, &QSlider::valueChanged, this, &volumeChange);
    volumeLabel = new QLabel("音量: 100%", musicSettings);
    volumeLabel->setStyleSheet(ButtonQSS::labelStyle2);
    volumeLabel->move(700, 120);
}

void Settings::setupBgUI() { // 背景ui实现
    bgSettings = new QWidget(this);
    prevBgButton = new QPushButton("◀", bgSettings);
    prevBgButton->setCursor(Qt::PointingHandCursor);
    prevBgButton->setStyleSheet(ButtonQSS::arrowButton);
    prevBgButton->move(250, 50);
    nextBgButton = new QPushButton("▶", bgSettings);
    nextBgButton->setCursor(Qt::PointingHandCursor);
    nextBgButton->setStyleSheet(ButtonQSS::arrowButton);
    nextBgButton->move(650, 50);
    currentBgLabel = new QLabel("Weiming Lake", bgSettings);
    currentBgLabel->setAlignment(Qt::AlignCenter);
    currentBgLabel->setStyleSheet(ButtonQSS::labelStyle);
    currentBgLabel->setGeometry(280, 50, 370, 50);
    prevBgLabel = new QLabel("(上一张)", bgSettings);
    prevBgLabel->setAlignment(Qt::AlignCenter);
    prevBgLabel->setStyleSheet(ButtonQSS::labelStyle2);
    prevBgLabel->setGeometry(150, 50, 100, 50);
    nextBgLabel = new QLabel("(下一张)", bgSettings);
    nextBgLabel->setAlignment(Qt::AlignCenter);
    nextBgLabel->setStyleSheet(ButtonQSS::labelStyle2);
    nextBgLabel->setGeometry(700, 50, 100, 50);
    connect(prevBgButton, &QPushButton::clicked, this, &on_prevBgButton_clicked);
    connect(nextBgButton, &QPushButton::clicked, this, &on_nextBgButton_clicked);
    // 切背景核心实现
    bgNames.push_back("Weiming Lake");
    bgNames.push_back("coniferous forest");
    bgNames.push_back("blank");
    
    bgUrls.push_back(":/images/background1.jpg");
    bgUrls.push_back(":/images/background2.png");
    bgUrls.push_back(":/images/background3.png");

    currentBgIndex = 0;
}

void Settings::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Escape) {
        emit exitSign();
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

void Settings::volumeChange(int value) {
    volumeLabel->setText(QString("音量: %1%").arg(value));
    setAOutput->setVolume((qreal) value / 100);
}

void Settings::on_prevSongButton_clicked() {
    --currentSongIndex;
    if(currentSongIndex < 0) {
        currentSongIndex += songUrls.size();
    }
    
    setPlayer->setSource(QUrl(songUrls[currentSongIndex]));
    setPlayer->play();
    currentSongLabel->setText(songNames[currentSongIndex]);
}

void Settings::on_nextSongButton_clicked() {
    ++currentSongIndex;
    if(currentSongIndex == songUrls.size()) {
        currentSongIndex = 0;
    }
    setPlayer->setSource(QUrl(songUrls[currentSongIndex]));
    setPlayer->play();
    currentSongLabel->setText(songNames[currentSongIndex]);
}

void Settings::on_prevBgButton_clicked() {
    --currentBgIndex;
    if(currentBgIndex < 0) {
        currentBgIndex += bgUrls.size();
    }
    setPixmap.load(bgUrls[currentBgIndex]);
    emit bgUpdateSign();
    currentBgLabel->setText(bgNames[currentBgIndex]);
}

void Settings::on_nextBgButton_clicked() {
    ++currentBgIndex;
    if(currentBgIndex == bgUrls.size()) {
        currentBgIndex = 0;
    }
    setPixmap.load(bgUrls[currentBgIndex]);
    emit bgUpdateSign();
    currentBgLabel->setText(bgNames[currentBgIndex]);
}