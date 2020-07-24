#include "mycoin.h"
#include <QString>
#include <QPixmap>
#include <QDebug>

MyCoin::MyCoin(QString iconImg)
{
    QPixmap pixmap;
    if(!pixmap.load(iconImg))
    {
        qDebug()<<"资源加载出错";
        return;
    }
    setFixedSize(50,50);    //适配金币框大小
    setStyleSheet("QPushButton{border:0px}");   //去除边框
    setIcon(pixmap);   //设置初始状态图片
    setIconSize(QSize(50,50));
    //初始化翻转定时器
    timer=new QTimer(this);
    //监听翻转信号，并翻转
    connect(timer,&QTimer::timeout,[=]()
    {
        changeCoin();
    });

}

void MyCoin::changeFlag()
{
    isAnimation=true;
    timer->start(20);   //开启动画定时器
    //翻转就交由上面的connect来做
}

void MyCoin::changeCoin()
{
    QPixmap pixmap;
    if(coinFlag)
    {
        QString str=QString(":/coins/res/Coin000%1.png").arg(minImg++);
        pixmap.load(str);
        setFixedSize(50,50);    //适配金币框大小
        setStyleSheet("QPushButton{border:0px}");   //去除边框
        setIcon(pixmap);   //设置图片
        setIconSize(QSize(50,50));
        if(minImg>maxImg)
        {
            minImg=1;
            coinFlag=false;
            isAnimation=false;
            timer->stop();
        }
    }
    else
    {
        QString str=QString(":/coins/res/Coin000%1.png").arg(maxImg--);
        pixmap.load(str);
        setFixedSize(50,50);    //适配金币框大小
        setStyleSheet("QPushButton{border:0px}");   //去除边框
        setIcon(pixmap);   //设置图片
        setIconSize(QSize(50,50));
        if(maxImg<minImg)
        {
            coinFlag=true;
            maxImg=8;
            isAnimation=false;
            timer->stop();
        }
    }
}

void MyCoin::mousePressEvent(QMouseEvent *event)
{
    //如果正在翻转或者已经胜利，禁用该按钮
    if(isAnimation || isWin)
    {
        return;
    }
    else
    {
        return QPushButton::mousePressEvent(event);
    }
}

