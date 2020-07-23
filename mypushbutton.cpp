#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>

MyPushButton::MyPushButton(QString normalImg,QString pressImg)
{
    //加载图片文件路径
    normalImgPath=normalImg;
    pressImgPath=pressImg;

    QPixmap pixmap;
    auto ret=pixmap.load(normalImgPath);
    if(!ret)
    {
        qDebug()<<"图片加载失败";
        return;
    }
    //设置图片固定大小，防止拖动
    setFixedSize(pixmap.width(),pixmap.height());
    setStyleSheet("QPushButton{border:0px}");//设置不规则图片样式
    setIcon(pixmap);//设置图标
    setIconSize(QSize(pixmap.width(),pixmap.height()));//让按钮大小保持为图片大小
}

void MyPushButton::zoomDownAnimation()
{
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");  //创建动画对象
    animation->setDuration(200);    //设置动画时间间隔（这个方法在其父类里）
    animation->setStartValue(QRect(x(),y(),width(),height()));  //设置起始位置（这个方法在其父类里）
    animation->setEndValue(QRect(x(),y()+10,width(),height()));//设置结束位置（这个方法在其父类里）
    animation->setEasingCurve(QEasingCurve::OutElastic); //设置弹跳曲线（这个方法在其父类里）
    animation->start();
}

void MyPushButton::zoomUpAnimation()
{
    //弹回动画与弹走动画是一样的，只不过是位置坐标不一样
    //注意，每次弹跳的动画都是以上一次动画后的图像所在坐标为基准
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    animation->setStartValue(QRect(x(),y()+10,width(),height()));
    animation->setEndValue(QRect(x(),y(),width(),height()));
    animation->setEasingCurve(QEasingCurve::OutElastic);
    animation->start();
}

void MyPushButton::mousePressEvent(QMouseEvent *event)
{
    //参照上面的代码让按下图重新加载即可
    if(pressImgPath!="")    //按下图片路径不为空，说明有切换图片
    {
        QPixmap pixmap;
        auto ret=pixmap.load(pressImgPath);
        if(!ret)
        {
            qDebug()<<"图片加载失败";
            return;
        }
        //设置图片固定大小，防止拖动
        setFixedSize(pixmap.width(),pixmap.height());
        setStyleSheet("QPushButton{border:0px}");//设置不规则图片样式
        setIcon(pixmap);//设置图标
        setIconSize(QSize(pixmap.width(),pixmap.height()));//让按钮大小保持为图片大小
    }
    //让其父类做除切换图片之外的事（渲染动画等）
    return QPushButton::mousePressEvent(event);
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(pressImgPath!="")    //按下图片路径不为空，说明有切换图片
    {
        QPixmap pixmap;
        auto ret=pixmap.load(normalImgPath);
        if(!ret)
        {
            qDebug()<<"图片加载失败";
            return;
        }
        //设置图片固定大小，防止拖动
        setFixedSize(pixmap.width(),pixmap.height());
        setStyleSheet("QPushButton{border:0px}");//设置不规则图片样式
        setIcon(pixmap);//设置图标
        setIconSize(QSize(pixmap.width(),pixmap.height()));//让按钮大小保持为图片大小
    }
    //让其父类做除切换图片之外的事（渲染动画等）
    return QPushButton::mouseReleaseEvent(event);
}







