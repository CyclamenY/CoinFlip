#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include "chooselevelscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainScene; }
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
        Q_OBJECT

    public:
        MainScene(QWidget *parent = nullptr);
        ~MainScene();
        void paintEvent(QPaintEvent *event) override;//重写painterEvent事件

    private:
        Ui::MainScene *ui;
        ChooseLevelScene *chooseScene=nullptr;  //初始化一个关卡选择场景备用
};
#endif // MAINSCENE_H
