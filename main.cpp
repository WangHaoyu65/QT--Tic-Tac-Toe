#include <QApplication>
#include <QDir>             //文件操作
#include <QStackedWidget>   //多页面切换（就是开始界面切游戏界面）
#include "StartScreen.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    // 初始化资源
    Q_INIT_RESOURCE(resourcejzq);

    QApplication a(argc, argv);

    // 设置当前工作目录
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // 创建页面堆叠容器
    QStackedWidget stack;

    StartScreen *start = new StartScreen;//开始界面
    Widget *game   = new Widget(nullptr, false); //双人对战
    Widget *aiGame = new Widget(nullptr, true);  //人机对战


    // 双人对战按钮:开始界面->游戏界面
    QObject::connect(start, &StartScreen::twoPlayerClicked, [&]()
    {
        stack.setCurrentWidget(game);
    });
    //人机对战按钮
    QObject::connect(start, &StartScreen::singlePlayerClicked, [&](){
            stack.setCurrentWidget(aiGame);
        });
    // 退出游戏按钮 → 退出应用
    QObject::connect(start, &StartScreen::exitClicked, &a, &QApplication::quit);

    stack.addWidget(start);
    stack.addWidget(game);
    stack.addWidget(aiGame);//将这三个页面添加入页面容器

    stack.setWindowTitle("QT版井字棋");
    stack.setFixedSize(320, 400);
    stack.show();

    return a.exec();
}
