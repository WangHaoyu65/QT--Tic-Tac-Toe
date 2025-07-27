#include "mylabel.h"
#include <QMouseEvent>
#include <QStyle>       //QT刷新
#include <QPixmap>      //图像显示

//myLabel的具体实现
myLabel::myLabel(int index, QWidget *parent)
    : QLabel(parent), m_index(index)    //将QLabel=parent  ,  m_index=index
{
    setFrameStyle(QFrame::Panel);   //绘制边框
    setAlignment(Qt::AlignCenter);  //居中显示
    setScaledContents(true);        //自动缩放
    setMouseTracking(true);         //开启鼠标跟踪,即鼠标在标签上面和离开都会触发鼠标事件

    // 设置统一样式，包含圆角、边框，悬停时的淡蓝背景
    setStyleSheet(R"(
        myLabel
        {
            border: 1px solid #cccccc;
            border-radius: 15px;
            background-color: transparent;//背景透明
        }

        myLabel[hover="true"]
        {
            background-color: rgba(200, 200, 255, 80);//鼠标悬停高亮
        }
    )");
}


//event是鼠标事件类的一个实例
//鼠标点击函数会在鼠标点击 myLabel 标签控件本身时触发
void myLabel::mousePressEvent(QMouseEvent *event)
{
    //pixmap用于保存标签的像素,如果像素未设置返回空
    //如果当前触发事件的是鼠标左键，并且这个标签没有显示图片（是空格子）的话，就发出 clicked 信号。
    if (event->button() == Qt::LeftButton && pixmap() == nullptr)
    {
        //返回信号"我这个标签被点了，而且编号是 m_index"
        emit clicked(m_index);
    }
}


//鼠标进入控件区域时就会被自动调用
void myLabel::enterEvent(QEvent *event)
{
    //如果当前这个标签 是空的格子（没图）
    if (!this->pixmap())
    {
        //为这个控件设置一个名为 "hover" 的动态属性，并赋值为 true
        this->setProperty("hover", true);
        //刷新以改变背景
        this->style()->unpolish(this);
        this->style()->polish(this);
    }
    //调用父类的默认行为,可以保证 QLabel 自带的事件处理逻辑（比如设置光标等）仍然有效。
    QLabel::enterEvent(event);
}


//鼠标离开标签区域时该函数被调用，原理和上面类似
void myLabel::leaveEvent(QEvent *event)
{
    this->setProperty("hover", false);
    this->style()->unpolish(this);
    this->style()->polish(this);
    QLabel::leaveEvent(event);
}
