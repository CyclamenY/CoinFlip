#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "playscene.h"

class ChooseLevelScene : public QMainWindow
{
        Q_OBJECT
    public:
        explicit ChooseLevelScene(QWidget *parent = nullptr);
        void paintEvent(QPaintEvent *event) override;
    private:
        //游戏场景对象指针
        PlayScene *playScene=nullptr;
    signals:
        //写一个自定义信号，告诉主场景点击了返回
        void chooseSceneBack(); //自定义信号只需要声明，不需要实现
};

#endif // CHOOSELEVELSCENE_H
