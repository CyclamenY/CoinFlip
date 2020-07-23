#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
        Q_OBJECT
    public:
        //禁用原本的构造函数
        //explicit MyPushButton(QWidget *parent = nullptr);
        //创建一个带有常规图片路径和被按下图片路径的构造函数
        //其中按下图片路径默认为空
        MyPushButton(QString normalImg,QString pressImg="");
        //弹起与跳下特效
        void zoomDownAnimation();
        void zoomUpAnimation();
        //重写按钮的按下与释放事件
        void mousePressEvent(QMouseEvent *) override;
        void mouseReleaseEvent(QMouseEvent *) override;
    private:
        QString normalImgPath;  //常规图标路径
        QString pressImgPath;   //被按下后图标路径


    signals:

};

#endif // MYPUSHBUTTON_H
