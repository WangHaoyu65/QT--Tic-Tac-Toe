#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>      //窗口控件
#include <QPushButton>  //按钮控件
#include <QLabel>       //标签(图片)控件
#include <QVBoxLayout>  //垂直布局管理

// 开始界面
class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);

signals:
    void twoPlayerClicked();    //双人对战
    void singlePlayerClicked(); //人机对战
    void exitClicked();         //退出游戏

private:
    QLabel *titleLabel;        //游戏标题
    QPushButton *btnTwoPlayer; //双人对战按钮
    QPushButton *btnSinglePlayer;//人机对战按钮
    QPushButton *btnExit;      //退出按钮

    void setupUi();            //构建界面布局
    void setupConnections();   //连接信号槽
};

#endif // STARTSCREEN_H
