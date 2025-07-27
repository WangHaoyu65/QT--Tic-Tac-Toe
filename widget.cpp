#include "widget.h"
#include <QVBoxLayout>  //垂直布局(棋盘在上，状态标签在下)
#include <QGridLayout>  //网格布局
#include <QMessageBox>  //弹窗
#include <QPixmap>      //图片显示
#include <QUrl>         //音效Url资源
#include <QDebug>
#include <QPainter>     //将已有图片资源绘制成圆角矩形
#include <QPainterPath> //提供圆角矩形路径
#include <QCoreApplication>   //退出应用
#include <QTime>        //记录和计算时间点或时间间隔
#include <QTimer>       //定时触发某个动作（定时器）

Widget::Widget(QWidget *parent, bool aiMode)
    : QWidget(parent)
    , m_aiMode(aiMode)
    , currentPlayer('O')
    , gameOver(false)//先手：O，同时把人机标识符跨作用域传输
{

    qsrand(QTime::currentTime().msec());//随机
    //初始化音效
    moveSound = new QSoundEffect(this);
    moveSound->setSource(QUrl("qrc:/resourcess/move.wav"));
    moveSound->setVolume(0.8f);

    winSound = new QSoundEffect(this);
    winSound->setSource(QUrl("qrc:/resourcess/win.wav"));
    winSound->setVolume(0.8f);

    //自定义函数,初始化棋盘中的小格子
    initBoard();

    //创建主布局
    QVBoxLayout *mainLay = new QVBoxLayout(this);
    QGridLayout *gridLay = new QGridLayout;

    for (int i = 0; i < 9; ++i)
        gridLay->addWidget(cells[i], i / 3, i % 3);//把格子载入布局

    gridLay->setSpacing(0);//格子之间无间距
    gridLay->setMargin(0); //棋盘窗口无边距

    statusLabel = new QLabel(this);             //显示"轮到哪一位玩家的信息"
    statusLabel->setText("当前：玩家 O");       //设置标签内文字
    statusLabel->setAlignment(Qt::AlignCenter);//居中
    statusLabel->setFixedHeight(30);           //标签高度30像素

    //垂直布局:中添加棋盘和状态标签
    mainLay->addLayout(gridLay);
    mainLay->addWidget(statusLabel);
    mainLay->setMargin(5);
    setLayout(mainLay);

    //设置窗口
    setWindowTitle(m_aiMode ? "人机对战 - QT版井字棋" : "QT版井字棋");
    setFixedSize(320, 400);

    //保证子页面也能画背景
    this->setAttribute(Qt::WA_StyledBackground);
    //背景渐变
    this->setStyleSheet(R"(
        QWidget {
            background: qlineargradient(
                spread:pad, x1:0, y1:0, x2:1, y2:1,
                stop:0 #f0fbfd, stop:1 #d9f3f9
            );
        }
    )");
}

Widget::~Widget() {}

void Widget::initBoard()
{
    //对棋盘中的每一个小格子进行初始化
    for (int i = 0; i < 9; ++i)
    {
        cells[i] = new myLabel(i, this);
        cells[i]->setFixedSize(100, 100);
        cells[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        //设置样式：圆角边框+白底+灰色边
        cells[i]->setStyleSheet(
            "QLabel {"
            " border: 2px solid #888;"
            " border-radius: 12px;"
            " background-color: #ffffff;"
            "}"
            "QLabel:hover {"
            " background-color: #f0f0f0;"
            "}"
        );
        //信号与槽的触发
        connect(cells[i], &myLabel::clicked, this, &Widget::onCellClicked);
    }
}

//槽函数
void Widget::onCellClicked(int idx)
{
    //如果游戏结束或者是当前格子中已经有东西了,就不做显示操作,直接返回
    if (gameOver || cells[idx]->pixmap()) return;

    moveSound->play();

    QString imgPath = (currentPlayer == 'O') ?
        ":/resourcess/O.png" : ":/resourcess/X.png";
    QPixmap original(imgPath);
    // 缩放图像到格子大小
    original = original.scaled(cells[idx]->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 创建带圆角的 QPixmap
    QPixmap rounded(original.size());                       //画布对象
    rounded.fill(Qt::transparent);                          //透明
    QPainter painter(&rounded);                             //绘图对象
    painter.setRenderHint(QPainter::Antialiasing, true);    //抗锯齿
    QPainterPath path;                                      //形状对象
    path.addRoundedRect(rounded.rect(), 12, 12);            //圆角矩形
    painter.setClipPath(path);                              //裁剪
    painter.drawPixmap(0, 0, original);                     //把图片导入画布
    cells[idx]->setPixmap(rounded);                         //设置格子内图片
    cells[idx]->setObjectName(QString(currentPlayer));      //标记当前玩家


    if (checkWin(currentPlayer))
    {
        endGame(QString("玩家 %1 胜利!").arg(currentPlayer), true);
        return;
    }
    if (isBoardFull())
    {
        endGame("平局!", false);
        return;
    }

    switchPlayer();
    if (m_aiMode && !gameOver && currentPlayer == 'X')
        {
            statusLabel->setText("AI 正在思考...");
            QTimer::singleShot(500, this, [this]()
            {makeAIMove();});//AI出棋延迟500ms
        }

}

void Widget::makeAIMove()
{
    QVector<int> empties;//容器存放空位
    for (int i = 0; i < 9; ++i)
        if (!cells[i]->pixmap()) empties.append(i);
    if (empties.isEmpty()) return;

    int choice = empties.at(qrand() % empties.size());
    // 随机选择，如果后续要升级AI算法就修改这里
    onCellClicked(choice);
}


void Widget::switchPlayer()
{
    currentPlayer = (currentPlayer == 'O' ? 'X' : 'O');
    statusLabel->setText(QString("当前：玩家 %1").arg(currentPlayer));
}

bool Widget::checkWin(char p)
{
    static const int wining[8][3] =
    {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };//所有win的情况
    for (auto &line : wining)
    {
        if (cells[line[0]]->objectName() == QString(p) &&
            cells[line[1]]->objectName() == QString(p) &&
            cells[line[2]]->objectName() == QString(p))
            return true;
    }//判断连成一条线的三个格子是不是属于同一个玩家
    return false;
}

bool Widget::isBoardFull()
{
    for (int i = 0; i < 9; ++i)
        if (cells[i]->pixmap() == nullptr)
            return false;
    return true;
}

//显示游戏结算页面
void Widget::endGame(const QString &msg, bool win)
{
    gameOver = true;

    if (win) winSound->play();


    auto ret = QMessageBox::question(this,
        "游戏结束", msg + "\n重新开始？",
        QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        resetBoard();
    } else {
        QCoreApplication::quit(); // 选“否”后退出
    }
}


void Widget::resetBoard()
{
    for (int i = 0; i < 9; ++i) {
        cells[i]->clear();
        cells[i]->setObjectName("");
    }
    gameOver = false;
    currentPlayer = 'O';
    statusLabel->setText("当前：玩家 O");
}
