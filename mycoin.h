#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
        Q_OBJECT
    public:
        MyCoin(QString iconImg);    //参数代表传入的金币还是银币的路径
        void changeFlag();   //改变标标识的函数
        void changeCoin();
        //重写按下事件
        void mousePressEvent(QMouseEvent *event) override;
        //添加金币自身属性
        int posX;       //硬币的矩阵x位置
        int posY;       //硬币的矩阵y位置
        bool coinFlag;  //硬币的正反标识
        bool isWin=false;   //胜利标识，用于禁用按钮
    private:
        QTimer *timer;  //动画定时器
        int minImg=1;   //最小图片编号
        int maxImg=8;   //最大图片编号
        bool isAnimation=false; //是否在动画中的标识
    signals:

};

#endif // MYCOIN_H
