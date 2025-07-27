//主界面
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSoundEffect>
#include "mylabel.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr, bool aiMode = false);
    ~Widget();

private slots:
    void onCellClicked(int idx);//槽函数，idx标识被点击的格子序号

private:
    myLabel *cells[9];      //cells中的每一个元素表示棋盘中的一个格子
    QLabel  *statusLabel;   //当前状态
    char currentPlayer;     //当前玩家
    bool gameOver;
    bool m_aiMode; //人机对战标志
    bool aiMode;
    QSoundEffect *moveSound;
    QSoundEffect *winSound;

    void initBoard();
    void switchPlayer();
    bool checkWin(char player);
    bool isBoardFull();
    void endGame(const QString &msg, bool win);
    void resetBoard();
    void makeAIMove();
};

#endif // WIDGET_H
