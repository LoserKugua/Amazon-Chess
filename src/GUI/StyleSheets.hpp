#ifndef STYLESHEETS_HPP
#define STYLESHEETS_HPP
// 头文件保护忘加了 幽默

// 为什么不用qss文件呢。
// 因为qss文件上次远程部署好像有点问题来着。
// 编译时间也就10s不到的事
// 好的我收回这句话，现在加了BGM之后build时间来到了1分钟，请输入文本
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>

namespace ButtonQSS {
    extern QString exitStyle;
    extern QString saveStyle;
    extern QString loadStyle;
    extern QString undoStyle;
    extern QString tipsStyle;
    extern QString menuStyle;
    extern QString settingsStyle;
    
    extern QString defaultButton;
    extern QString arrowButton;
    // extern QString
    extern QString volumeSlider;
    extern QString labelStyle;
    extern QString labelStyle2;
    extern QString titleLabelStyle;
    void shadowInit(QPushButton* shadowButton);
}

#endif