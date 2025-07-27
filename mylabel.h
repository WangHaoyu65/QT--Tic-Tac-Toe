#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>  //鼠标处理

//创建一个myLabel的自定义类
class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(int index, QWidget *parent = nullptr);  //myLabel的构造函数,传入index,和父类
    int index() const { return m_index; }  //index标识棋盘格子的编号

signals:
    void clicked(int index);   //点击信号

protected:
    void mousePressEvent(QMouseEvent *event) override;   //鼠标点击函数
    void enterEvent(QEvent *event) override;  //鼠标函数
    void leaveEvent(QEvent *event) override;  //鼠标释放函数


private:
    int m_index;     //myLabel里面的成员 m_index
};

#endif // MYLABEL_H
