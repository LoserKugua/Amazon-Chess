#include "StyleSheets.hpp"

void ButtonQSS::shadowInit(QPushButton* shadowButton) {
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(shadowButton);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    shadowEffect->setOffset(5, 5);
    shadowButton->setGraphicsEffect(shadowEffect);
}

namespace ButtonQSS {
    QString exitStyle = "QPushButton {"
        "    font-size: 35px;"
        "    font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;"
        "    color: #FFFFFF;"
        "    background-color: #E74C3C;"
        "    border: none;"
        "    border-radius: 15px;"
        "    padding: 20px 40px;"
        "    margin: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #C0392B;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #A93226;"
        "    padding: 22px 38px 18px 42px;" /* 模拟下沉效果 */
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDC3C7;"
        "    color: #ECF0F1;"
        "}";
    QString saveStyle = "QPushButton {"
        "    font-size: 35px;"
        "    font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;"
        "    color: #FFFFFF;"
        "    background-color: #3ce73c;"
        "    border: none;"
        "    border-radius: 15px;"
        "    padding: 20px 40px;"
        "    margin: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #35c02b;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #26a92d;"
        "    padding: 22px 38px 18px 42px;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDC3C7;"
        "    color: #ECF0F1;"
        "}";
    QString loadStyle = "QPushButton {"
        "    font-size: 35px;"
        "    font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;"
        "    color: #FFFFFF;"
        "    background-color: #e4e73c;"
        "    border: none;"
        "    border-radius: 15px;"
        "    padding: 20px 40px;"
        "    margin: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c0c02b;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #a9a926;"
        "    padding: 22px 38px 18px 42px;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDC3C7;"
        "    color: #ECF0F1;"
        "}";
    QString undoStyle = "QPushButton {"
        "    font-size: 35px;"
        "    font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;"
        "    color: #FFFFFF;"
        "    background-color: #a83ce7;"
        "    border: none;"
        "    border-radius: 15px;"
        "    padding: 20px 40px;"
        "    margin: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #982bc0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #a926a7;"
        "    padding: 22px 38px 18px 42px;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDC3C7;"
        "    color: #ECF0F1;"
        "}";
    QString tipsStyle = "QPushButton {"
        "    font-size: 35px;"
        "    font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;"
        "    color: #FFFFFF;"
        "    background-color: #3c59e7;"
        "    border: none;"
        "    border-radius: 15px;"
        "    padding: 20px 40px;"
        "    margin: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2b5dc0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #263ea9;"
        "    padding: 22px 38px 18px 42px;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDC3C7;"
        "    color: #ECF0F1;"
        "}";
    QString settingsStyle = "QPushButton {"
        "    font-size: 35px;"
        "    font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;"
        "    color: #FFFFFF;"
        "    background-color: #3ce4e7;"
        "    border: none;"
        "    border-radius: 15px;"
        "    padding: 20px 40px;"
        "    margin: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2bbec0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #2698a9;"
        "    padding: 22px 38px 18px 42px;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #BDC3C7;"
        "    color: #ECF0F1;"
        "}";
    QString menuStyle = R"qss(
        QPushButton {
            font-family: 'Microsoft YaHei UI', 'Arial', sans-serif;
            font-size: 10pt;
            font-weight: 500;
            color: black;
            border-width: 1px;
            border-style: solid;
            border-color: black;
            border-radius: 0px;
            background-color: #d8d8d8;
            padding: 5px 10px;
            margin: 2px;
        }
        QPushButton:hover {
            color: white;
            border-color: white;
            background-color: #55FF55;
        }
        QPushButton:pressed {
            color: white;
            border-color: white;
            background-color: #33DD33;
            padding: 6px 9px 4px 11px;
        })qss";
    
        
    QString defaultButton = R"(
        QPushButton {
            background-color: #383838;
            border: 2px solid #555555;
            color: #FFFFFF;
            padding: 8px 16px;
            border-radius: 8px;
            font-family: "Segoe UI", "Tahoma", sans-serif;
            font-size: 14px;
            font-weight: bold;
            text-align: center;
        }
        QPushButton:hover {
            background-color: #555555; /* 悬停时颜色变深 */
            border-color: #888888;
            color: #FFFF00; /* 悬停时黄色文字 */
        }
        QPushButton:pressed {
            background-color: #2a2a2a; /* 按下时颜色更深 */
            border-color: #aaaaaa;
            padding-top: 10px; /* 模拟按压效果 */
        }
        QPushButton:disabled {
            background-color: #222222;
            border-color: #444444;
            color: #777777;
        }
    )";
    QString arrowButton = R"(
        QPushButton {
            background-color: #bb00ff;
            border: none;
            color: #ffff00;
            font-family: "Segoe UI", "Tahoma", sans-serif;
            font-size: 50px;
            font-weight: bolder;
            padding: 0px;
            margin: 0px;
        }
        QPushButton:hover {
            color: #00AACC;
        }
        QPushButton:pressed {
            color: #007799;
        }
    )";
    QString volumeSlider = R"(
        QSlider::groove:horizontal {
            height: 6px;
            background: #c0c0c0;
            border-radius: 3px;
        }
        QSlider::handle:horizontal {
            width: 16px;
            height: 16px;
            margin: -5px 0;
            background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,
                                stop:0.0 rgba(255, 255, 255, 1), stop:0.9 rgba(255, 255, 255, 1));
            border: 1px solid #334;
            border-radius: 8px;
        }
        QSlider::add-page:horizontal {
            background: rgba(255, 255, 255, 1);
            border-radius: 3px;
        }
        QSlider::sub-page:horizontal {
            background: #00a2ff;
            border-radius: 3px;
        }
    )";
    QString labelStyle = R"(
        QLabel {
            color: #FFFFFF;
            font-family: "Segoe UI", "Tahoma", sans-serif;
            font-size: 30px;
            font-weight: normal;
            background-color: transparent;
            text-align: center;
        }
    )";
    QString labelStyle2 = R"(
        QLabel {
            color: #FFFFFF;
            font-family: "Segoe UI", "Tahoma", sans-serif;
            font-size: 25px;
            font-weight: normal;
            background-color: transparent;
            text-align: center;
        }
    )";
    QString titleLabelStyle = R"(
        QLabel {
            color: #00AACC;
            font-family: "Kaiti", "Arial", sans-serif;
            font-size: 36px;
            font-weight: bold;
            background-color: transparent;
            text-align: center;
        }
    )";
}