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
#include <QPropertyAnimation>
#include <QSound>

PlayScene::PlayScene(int levelNum)
{
    levelIndex=levelNum;
    //初始化游戏场景
    setFixedSize(320,588);
    setWindowIcon(QIcon(":/coins/res/Coin0001.png"));
    setWindowTitle(tr("开始游玩吧！"));
    QMenuBar *mBar=menuBar();
    setMenuBar(mBar);   //加入菜单栏
    auto MenuStart=mBar->addMenu(tr("开始"));
    auto actionQuit=MenuStart->addAction(tr("退出")); //加入退出动作
    connect(actionQuit,&QAction::triggered,[=]()
    {
        close();
    });
    //添加音效
        //设置音效
    playBGM->setLoops(-1);
    playBGM->play();
        //添加翻金币音效
    QSound *flipSound=new QSound(":/wav/res/ConFlipSound.wav",this);
        //添加返回按键音效
    QSound *backSound=new QSound(":/wav/res/BackButtonSound.wav",this);
    //放置返回按钮
    MyPushButton *backButton=new MyPushButton(":/button/res/BackButton.png",":/button/res/BackButtonSelected.png");
    backButton->setParent(this);
    backButton->move(width()-backButton->width()-10,height()-backButton->height()-10);
    //监听返回按钮按下事件
    connect(backButton,&MyPushButton::clicked,[=]()
    {
        //qDebug()<<"返回";
        //播放返回音效
        backSound->play();
        //这里需要返回到主场景，需要发送一个信号通知关卡选择场景让其显示
        QTimer::singleShot(180,this,[=]()
        {
            playBGM->stop();
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
    font.setPointSize(20);  //设置字号
    QPalette palette;   //这里设置颜色不能直接写在构造函数里，会导致显示不正常
    palette.setColor(QPalette::WindowText,QColor(102,204,255));//#66ccff颜色
    QLabel *levelLabel=new QLabel(str,this);    //创建的label默认没有大小
    levelLabel->setPalette(palette);
    levelLabel->setFont(font);
    //设置label大小且移动到指定区域
    levelLabel->setGeometry(30,height()-60,160,50);//前二参数为左上角坐标，后俩参数为宽和高
    //加载关卡数据
    dataConfig config(this);
    for(int i=0;i<4;++i)
    {
        auto data=config.mData[levelIndex];
        for(int j=0;j<4;++j)
        {
            levelData[i][j]=data[i][j];
        }
    }
    //胜利图片加载
    QPixmap winMap(":/background/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,winMap.width(),winMap.height());
    winLabel->setPixmap(winMap);
    winLabel->move((width()-winMap.width())/2,-winMap.height());
    //失败图片加载
    QPixmap failMap(":/background/res/Fail.png");
    failLabel->setGeometry(0,0,failMap.width(),failMap.height());
    failLabel->setPixmap(failMap);
    failLabel->move((width()-failMap.width())/2,-failMap.height());
    //放置金币背景板
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
                str=":/coins/res/Coin0001.png";
            }
            else
            {
                str=":/coins/res/Coin0008.png";
            }
            MyCoin *coin=new MyCoin(str);
            coin->setParent(this);
            coin->move(60+i*50,194+j*50);
            //赋予金币属性
            coin->posX=i;
            coin->posY=j;
            coin->coinFlag=levelData[i][j]; //1为正面 0为反面
            //将金币按钮放入专用二维数组里，以便后期维护
            buttonCoin[i][j]=coin;
            //点击金币进行翻转
            connect(coin,&MyCoin::clicked,[=]()
            {
                //播放翻转音效
                flipSound->play();
                //禁用所有金币按钮
                for(int i=0;i<4;++i)
                {
                    for(int j=0;j<4;++j)
                    {
                        buttonCoin[i][j]->isWin=true;
                    }
                }
                coin->changeFlag();
                levelData[i][j]=levelData[i][j]==1?0:1;
                //翻转周围
                changeRoundCoin(coin);
            });
        }
    }
    levelTime=config.gameTime[levelIndex-1];    //读入关卡时间
    QLabel *timeLabel=new QLabel(QString::number(levelTime),this);
    timeLabel->setAlignment(Qt::AlignHCenter);
    timeLabel->setPalette(palette);
    timeLabel->setFont(font);
    timeLabel->setGeometry(130,height()-150,60,50);
    //定时器初始化
    gameTimer->start(std::chrono::seconds(1));
    connect(gameTimer,&QTimer::timeout,[=]()
    {
        char timeRemain[10];
        memset(timeRemain,0,10);
        itoa(levelTime,timeRemain,10);
        timeLabel->setText(timeRemain);
        if(--levelTime<=0)
        {
            failAnimation();
            //禁用所有金币按钮
            for(int i=0;i<4;++i)
            {
                for(int j=0;j<4;++j)
                {
                    buttonCoin[i][j]->isWin=true;
                }
            }
        }
    });
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

void PlayScene::changeRoundCoin(MyCoin *coin)
{
    //延时翻转
    QTimer::singleShot(300,this,[=]()
    {
        //翻转右侧
        if(coin->posX+1<4)
        {
            buttonCoin[coin->posX+1][coin->posY]->changeFlag();
            levelData[coin->posX+1][coin->posY]=levelData[coin->posX+1][coin->posY]==1?0:1;
        }
        //翻转左侧
        if(coin->posX-1>-1)
        {
            buttonCoin[coin->posX-1][coin->posY]->changeFlag();
            levelData[coin->posX-1][coin->posY]=levelData[coin->posX-1][coin->posY]==1?0:1;
        }
        //翻转下侧
        if(coin->posY+1<4)
        {
            buttonCoin[coin->posX][coin->posY+1]->changeFlag();
            levelData[coin->posX][coin->posY+1]=levelData[coin->posX][coin->posY+1]==1?0:1;
        }
        //翻转上侧
        if(coin->posY-1>-1)
        {
            buttonCoin[coin->posX][coin->posY-1]->changeFlag();
            levelData[coin->posX][coin->posY-1]=levelData[coin->posX][coin->posY-1]==1?0:1;
        }
        isWin=true;
        for(int i=0;i<4;++i)
        {
            for(int j=0;j<4;++j)
            {
                if(levelData[i][j]==0)
                {
                    isWin=false;
                    break;
                }
            }
            if(!isWin)
            {
                break;
            }
        }
        //解禁所有金币按钮
        for(int i=0;i<4;++i)
        {
            for(int j=0;j<4;++j)
            {
                buttonCoin[i][j]->isWin=false;
            }
        }
        if(isWin)
        {
            //qDebug()<<"胜利";
            //播放胜利音效
            playBGM->stop();
            winSound->play();
            gameTimer->stop();
            for(int i=0;i<4;++i)
            {
                for(int j=0;j<4;++j)
                {
                    buttonCoin[i][j]->isWin=true;
                }
            }
            winAnimation();
        }
    });
}

void PlayScene::winAnimation()
{

    //将胜利的图片移动下来
    QPropertyAnimation *animation=new QPropertyAnimation(winLabel,"geometry");
    //设置时间间隔
    animation->setDuration(1000);
    //设置开始位置
    animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
    //设置结束位置
    animation->setEndValue(QRect(winLabel->x(),winLabel->y()+175,winLabel->width(),winLabel->height()));
    //设置缓和曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}

void PlayScene::failAnimation()
{

    //将胜利的图片移动下来
    QPropertyAnimation *animation=new QPropertyAnimation(failLabel,"geometry");
    //设置时间间隔
    animation->setDuration(1000);
    //设置开始位置
    animation->setStartValue(QRect(failLabel->x(),failLabel->y(),failLabel->width(),failLabel->height()));
    //设置结束位置
    animation->setEndValue(QRect(failLabel->x(),failLabel->y()+175,failLabel->width(),failLabel->height()));
    //设置缓和曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}
