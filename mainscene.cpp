#include "mainscene.h"
#include "ui_mainscene.h"
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include "mypushbutton.h"
#include <QTimer>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    setFixedSize(320,588);  //设置固定大小
    setWindowIcon(QIcon(":/animation/gold/res/Coin0001.png"));//设置窗口图标
    setWindowTitle(tr("来翻金币吧！"));
    connect(ui->actionQuit,&QAction::triggered,[=]()
    {
        close();
    });
    //开始按钮
    MyPushButton *startButton=new MyPushButton(":/button/res/MenuSceneStartButton.png");
    startButton->setParent(this);   //让其在本窗口中显示
    startButton->move(width()/2-startButton->width()/2,height()*0.618); //将开始按钮移动至指定位置
    chooseScene=new ChooseLevelScene;   //创建出关卡场景（唯一且不变）
    //监听按下事件
    //监听选择关卡按钮的返回信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=]()
    {
        //当关卡选择界面返回被按下，由于关卡选择界面唯一，所以只需要将其隐藏
        //下一次再进入时直接show就可以了
        show();
    });
    connect(startButton,&QPushButton::clicked,[=]()
    {
        //做一个弹起特效
        startButton->zoomDownAnimation();   //弹
        startButton->zoomUpAnimation();     //起
        //延时一段时间后进入关卡场景中
        //singleShot为QTimer的静态成员函数，相当于一个槽函数，能够实现定时短时间后使某对象发出某个动作
        QTimer::singleShot(180,this,[=]()
        {
            hide();     //自身隐藏
            chooseScene->show();    //关卡选择界面显示
        });
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap;
    pixmap.load(":/background/res/MenuSceneBg.png");
    painter.drawPixmap(0,0,width(),height(),pixmap);//拉伸填充屏幕
}
