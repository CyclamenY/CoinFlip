#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QObject>
#include <QMap>
#include <QVector>

class dataConfig : public QObject
{
        Q_OBJECT
    public:
        explicit dataConfig(QObject *parent = 0);

    public:
        QMap<int,QVector<QVector<int>>> mData;   //创建一个关卡和关卡数据键值对
        QVector<int> gameTime;


    signals:

};

#endif // DATACONFIG_H
