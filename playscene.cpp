#include "playscene.h"
#include <QMenuBar>
#include "mypushbutton.h"
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QFont>
#include <QPalette>
#include "mycoin.h"
#include "dataconfig.h"
#include "chooselevelscene.h"
#include "fontmanager.h"


PlayScene::PlayScene(int levelNum)
{
    levelIndex=levelNum;
    //初始化游戏场景
    setFixedSize(320,588);
    setWindowIcon(QIcon(":/animation/gold/res/Coin0001.png"));
    setWindowTitle(tr("开始游玩吧！"));
    QMenuBar *mBar=menuBar();
    setMenuBar(mBar);   //加入菜单栏
    auto MenuStart=mBar->addMenu(tr("开始"));
    auto actionQuit=MenuStart->addAction(tr("退出")); //加入退出动作
    connect(actionQuit,&QAction::triggered,[=]()
    {
        close();
    });
    //放置返回按钮
    MyPushButton *backButton=new MyPushButton(":/button/res/BackButton.png",":/button/res/BackButtonSelected.png");
    backButton->setParent(this);
    backButton->move(width()-backButton->width()-10,height()-backButton->height()-10);
    //监听返回按钮按下事件
    connect(backButton,&MyPushButton::clicked,[=]()
    {
        //qDebug()<<"返回";
        //这里需要返回到主场景，需要发送一个信号通知关卡选择场景让其显示
        QTimer::singleShot(180,this,[=]()
        {
            emit playSceneBack();
            //hide();//这里不要隐藏，因为在选择界面会直接析构掉这个画面，隐藏了程序会报错的
        });
    });
    //显示当前选择的关卡数
    QString str=QString(tr("当前难度:%1")).arg(levelIndex);
    //加载字体
    FontManager *fontmanager=FontManager::getInstance(this);
    QFont font;
    if (!fontmanager->m_fontList.isEmpty())
    {
        font.setFamily(fontmanager->m_fontList.at(0));
    }
    font.setPointSize(16);  //设置字号
    QPalette palette;   //这里设置颜色不能直接写在构造函数里，会导致显示不正常
    palette.setColor(QPalette::WindowText,QColor(102,204,255));//#66ccff颜色
    QLabel *levelLabel=new QLabel(str,this);    //创建的label默认没有大小
    levelLabel->setPalette(palette);
    levelLabel->setFont(font);
    //设置label大小且移动到指定区域
    levelLabel->setGeometry(30,height()-60,120,50);//前二参数为左上角坐标，后俩参数为宽和高
    //加载关卡数据
    dataConfig config(this);
    QVector<QVector<int>> levelData(4,QVector<int>(4));
    for(int i=0;i<4;++i)
    {
        auto data=config.mData[levelIndex];
        for(int j=0;j<4;++j)
        {
            levelData[i][j]=data[i][j];
        }
    }
    QPixmap pixmap(":/background/res/BoardNode.png");
    for(int i=0;i<4;++i)
    {
        for(int j=0;j<4;j++)
        {
            //设置金币区域背景
            QLabel *iconBackground=new QLabel(this);
            iconBackground->setFixedSize(50,50);    //设置大小
            iconBackground->setPixmap(pixmap);
            iconBackground->move(60+i*50,194+j*50); //移动位置
            //创建金币或银币
            if(levelData[i][j]==1)
            {
                MyCoin *goldCoin=new MyCoin(":/animation/gold/res/Coin0001.png");
                goldCoin->setParent(this);
                goldCoin->move(60+i*50,194+j*50);       //移动至金币框
            }
            else
            {
                MyCoin *sliverCoin=new MyCoin(":/animation/sliver/res/Coin0008.png");
                sliverCoin->setParent(this);
                sliverCoin->move(60+i*50,194+j*50);     //移动至银币框
            }
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap;
    pixmap.load(":/background/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,width(),height(),pixmap);//拉伸填充屏幕
    pixmap.load(":/background/res/Title.png");
    pixmap=pixmap.scaled(pixmap.width()*0.5,pixmap.height()*0.5);//进行缩放操作，返回一个新的pixmap
    painter.drawPixmap(10,30,pixmap);//画背景上的图标
}
