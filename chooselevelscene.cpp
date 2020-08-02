#include "chooselevelscene.h"
#include <QIcon>
#include <QPainter>
#include <QMenuBar>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QFont>
#include "fontmanager.h"
#include <QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    setFixedSize(320,588);
    setWindowIcon(QIcon(":/coins/res/Coin0001.png"));
    setWindowTitle(tr("选择难度！"));
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
    //选择关卡音效
    QSound *chooseSound=new QSound(":/wav/res/TapButtonSound.wav",this);
    //返回音效
    QSound *backSound=new QSound(":/wav/res/BackButtonSound.wav",this);
    //监听返回按钮按下事件
    connect(backButton,&MyPushButton::clicked,[=]()
    {
        backSound->play();
        //这里需要返回到主场景，需要发送一个信号通知主场景再显示
        QTimer::singleShot(180,this,[=]()
        {
            emit chooseSceneBack();
            chooseBGM->stop();
            hide(); //自身隐藏，不需要析构
        });
    });
    //加载字体
    FontManager *fontmanager=FontManager::getInstance(this);
    QFont font;
    if (!fontmanager->m_fontList.isEmpty())
    {
        font.setFamily(fontmanager->m_fontList.at(0));
    }
    //创建选择关卡按钮（单循环创建一个矩阵）
    for(int i=0;i<20;++i)
    {
        MyPushButton *levelButton=new MyPushButton(":/button/res/LevelIcon.png");
        levelButton->setParent(this);
        //行列排布与模除数是无关的，但是模除数要一致，否则排布会乱，现记作K
        //%与/的位置决定了行列排布
        //如果%在前，那么说明是(i/K)行K列，且排布方式为先行后列
        //如果/在前，那么说明是K行(i/K)列，且排布方式为先列后行
        //每个数前面的+值代表起始值，每个数的*值代表间距
        levelButton->move(25+i%4*70,150+i/4*70);
//        levelButton->setText(QString::number(i+1)); //这样显示效果很差
        //一般使用QLebal来创建文字
        QLabel *levelLabel=new QLabel(QString::number(i+1),this);
        levelLabel->setFont(font);
        levelLabel->setFixedSize(levelButton->width(),levelButton->height());//设置label和button的大小一样大
        levelLabel->move(25+i%4*70,150+i/4*70); //移动label
        //label默认text是悬挂左对齐的，需要将其设置为悬挂居中
        levelLabel->setAlignment(Qt::AlignCenter);
        //label会覆盖在button的上方导致clicked事件不能被正确触发，需要设置cliked事件能够无视上方的label
        levelLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        //监听每个按钮的点击事件
        connect(levelButton,&QPushButton::clicked,[=]()
        {
//            QString str=QString("选择第%1关").arg(i+1);
//            qDebug()<<str;
            chooseSound->play();
            chooseBGM->stop();
            //随用随创建的对象，与关卡选择界面的编写理念不一样
            //所以在游戏界面返回时的操作也会不一样，详见下面
            playScene=new PlayScene(i+1);
            QTimer::singleShot(80,this,[=]()
            {
                hide(); //自身隐藏
                playScene->setGeometry(geometry());
                playScene->show();//游戏场景显示
            });
            connect(playScene,&PlayScene::playSceneBack,[=]()
            {
                setGeometry(playScene->geometry());
                show();
                chooseBGM->play();
                delete playScene;
                playScene=nullptr;
                //当点击返回时，程序会直接删除游戏场景
                //以便在下一次进入时创建一个新的游戏场景
            });
        });
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap;
    pixmap.load(":/background/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,width(),height(),pixmap);//拉伸填充屏幕
    //加载标题
    pixmap.load(":/background/res/Title.png");
    pixmap=pixmap.scaled(width()*0.9,pixmap.height());//进行缩放操作，返回一个新的pixmap
    painter.drawPixmap((width()-pixmap.width())/2,30,pixmap);//画背景上的图标
}
