#include "fontmanager.h"
#include <QFontDatabase>

FontManager *FontManager::m_instance=nullptr;

FontManager::FontManager(QObject *parent):QObject(parent)
{
    //加载字体
    auto FontID=QFontDatabase::addApplicationFont(":/font/res/PixFont.ttf"); // 从source资源文件
    m_fontList << QFontDatabase::applicationFontFamilies(FontID);
}
