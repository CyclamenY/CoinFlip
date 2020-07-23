#include "mycoin.h"
#include <QString>
#include <QPixmap>
#include <QDebug>

MyCoin::MyCoin(QString iconImg)
{
    QPixmap pix;
    if(!pix.load(iconImg))
    {
        qDebug()<<"资源加载出错";
        return;
    }
    setFixedSize(50,50);    //适配金币框大小
    setStyleSheet("QPushButton{border:0px}");   //去除边框
    setIcon(pix);   //设置初始状态图片
    setIconSize(QSize(50,50));
}
