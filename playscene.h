#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"
#include <QLabel>
#include <QSound>
#include <QTimer>

class PlayScene : public QMainWindow
{
        Q_OBJECT
    public:
        //explicit PlayScene(QWidget *parent = nullptr);
        PlayScene(int levelNum);
        void paintEvent(QPaintEvent *event) override;
    private:
        int levelIndex; //记录所选关卡
        int levelTime;
        int levelData[4][4];    //关卡的金币摆放信息
        MyCoin *buttonCoin[4][4];   //初始化完成后的金币矩阵信息，用于翻转周围
        bool isWin;     //判断是否胜利的标识
        QTimer *gameTimer=new QTimer(this);
        //添加音乐
        QSound *playBGM=new QSound(":/wav/res/playScene.wav",this);
        //添加胜利音效
        QSound *winSound=new QSound(":/wav/res/LevelWinSound.wav",this);
        QLabel *winLabel=new QLabel(this);      //胜利label，用于加载图片
        QLabel *failLabel=new QLabel(this);     //失败label，用于加载图片
        void changeRoundCoin(MyCoin *coin);
        void winAnimation();    //胜利动画
        void failAnimation();   //失败动画
    signals:
        void playSceneBack();
};


#endif // PLAYSCENE_H
