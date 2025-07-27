#include "StartScreen.h"
#include <QFont>    // 字体设置

StartScreen::StartScreen(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口可修改背景
    this->setAttribute(Qt::WA_StyledBackground);

    // 直接针对 this 设置样式，保证背景生效
    this->setStyleSheet(R"(
        background: qlineargradient(
            spread:pad, x1:0, y1:0, x2:1, y2:1,
            stop:0 #f0fbfd, stop:1 #d9f3f9
        );
        QPushButton {
            font-size: 14px;                           /* 字体大小 */
            padding: 8px;                              /* 内边距 */
            border: 2px solid #666;                    /* 边框大小颜色 */
            border-radius: 6px;                        /* 边框圆角 */
            background-color: rgba(255, 255, 255, 200); /* 按钮颜色 */
        }
        QPushButton:hover {
            background-color: rgba(230, 230, 230, 200); /* 鼠标悬停高亮时的按钮颜色 */
        }
    )");

    setupUi();          // 构建界面布局
    setupConnections(); // 连接信号槽
}

void StartScreen::setupUi()
{
    // 启动页面大标题
    titleLabel = new QLabel(tr("井字棋游戏"), this);
    QFont titleFont;
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #333;");

    // 按钮
    QFont btnFont;
    btnFont.setPointSize(16);
    btnTwoPlayer = new QPushButton(tr("双人对战"), this);
    btnTwoPlayer->setFont(btnFont);
    btnSinglePlayer= new QPushButton(tr("人机对决"), this);
    btnSinglePlayer->setFont(btnFont);
    btnExit = new QPushButton(tr("退出游戏"), this);
    btnExit->setFont(btnFont);

    // 垂直布局
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addStretch();            // 顶部空白
    lay->addWidget(titleLabel);
    lay->addSpacing(30);          // 标题和按钮之间空30像素
    lay->addWidget(btnTwoPlayer);
    lay->addSpacing(10);          // 按钮之间空10像素
    lay->addWidget(btnSinglePlayer);
    lay->addSpacing(10);
    lay->addWidget(btnExit);
    lay->addStretch();            // 底部空白

    // 设置按钮统一大小
    btnTwoPlayer->setFixedHeight(50);   // 2Player按钮高50像素
    btnSinglePlayer->setFixedHeight(50);
    btnExit->setFixedHeight(50);        // Exit按钮高50像素

    setLayout(lay);
}

void StartScreen::setupConnections() // connect槽
{
    connect(btnTwoPlayer, &QPushButton::clicked, this, &StartScreen::twoPlayerClicked);
    connect(btnSinglePlayer,&QPushButton::clicked, this, &StartScreen::singlePlayerClicked);
    connect(btnExit,      &QPushButton::clicked, this, &StartScreen::exitClicked);
}
