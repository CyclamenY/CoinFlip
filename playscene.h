#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QVector>

class PlayScene : public QMainWindow
{
        Q_OBJECT
    public:
        //explicit PlayScene(QWidget *parent = nullptr);
        PlayScene(int levelNum);
        void paintEvent(QPaintEvent *event) override;
    private:
        int levelIndex; //记录所选关卡
        QVector<QVector<int>> levelData;
    signals:
        void playSceneBack();
};

#endif // PLAYSCENE_H
