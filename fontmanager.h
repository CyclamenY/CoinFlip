#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QObject>

class FontManager : public QObject
{
        Q_OBJECT
    private:
        FontManager(QObject *parent = nullptr);
        static FontManager *m_instance;
    public:
        //explicit FontManager(QObject *parent = nullptr);
        static FontManager *getInstance(QObject *parent)
        {
            if(!m_instance)
            {
                m_instance=new FontManager(parent);
                static ReleseClass r1;
            }
            return m_instance;
        }
        class ReleseClass
        {
            public:
                ~ReleseClass()
            {
                if(FontManager::m_instance)
                {
                    delete FontManager::m_instance;
                    FontManager::m_instance=nullptr;
                }
            }
        };
        QStringList m_fontList;
    signals:

};


#endif // FONTMANAGER_H
