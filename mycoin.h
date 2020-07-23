#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    MyCoin(QString iconImg);    //参数代表传入的金币还是银币的路径

signals:

};

#endif // MYCOIN_H
