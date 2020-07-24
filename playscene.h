#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"
#include <QLabel>
#include <QSound>

class PlayScene : public QMainWindow
{
        Q_OBJECT
    public:
        //explicit PlayScene(QWidget *parent = nullptr);
        PlayScene(int levelNum);
        void paintEvent(QPaintEvent *event) override;
    private:
        int levelIndex; //记录所选关卡
        int levelData[4][4];    //关卡的金币摆放信息
        MyCoin *buttonCoin[4][4];   //初始化完成后的金币矩阵信息，用于翻转周围
        bool isWin;     //判断是否胜利的标识
        //添加胜利音效
        QSound *winSound=new QSound(":/wav/res/LevelWinSound.wav",this);
        QLabel *winLabel=new QLabel(this);
        void changeRoundCoin(MyCoin *coin);
        void winAnimation();
    signals:
        void playSceneBack();
};


#endif // PLAYSCENE_H
