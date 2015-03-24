#include "settings.h"


pSettings::pSettings() :
    isFirstRun(false)
    ,mSettings(0)
{

    mSettings = new QSettings("settings.ini", QSettings::IniFormat);
    loadSettings();

}

pSettings::~pSettings()
{
    saveSettings();

    if(mSettings != 0)
        delete mSettings;
}

void pSettings::saveSettings()
{
    mSettings->remove("MainWindow/font");
    mSettings->setValue("FirstRun/IsRunFirst",  true);
    mSettings->setValue("MainWindow/title",     mainWindowTitle);
    mSettings->setValue("MainWindow/position",  mainWindowRect);
    mSettings->setValue("MainWindow/color",     mainWindowColor);
    mSettings->setValue("MainWindow/font",      mainWindowFont.toString());
}



void pSettings::loadSettings()
{

    if(mSettings->value("FirstRun/IsRunFirst").toBool() == false)
        isFirstRun = true;

    mainWindowTitle = mSettings->value("MainWindow/title")              .toString();
    mainWindowRect  = mSettings->value("MainWindow/position")           .toRect();
    mainWindowColor = mSettings->value("MainWindow/color")              .toString();
    mainWindowFont.fromString(mSettings->value("MainWindow/font")       .toString());
}




void pSettings::setMainWindowTitle(const QString &aTitle)
{
    mainWindowTitle = aTitle;
}

void pSettings::setMainWindowRect(const QRect &aRect)
{
    mainWindowRect = aRect;
}

void pSettings::setMainWindowColor(const QColor &aColor)
{
    mainWindowColor = aColor;
}

void pSettings::setMainWindowFont(const QFont &aFont)
{
    mainWindowFont = aFont;
}

void pSettings::setBoardFont(const QFont &bFont)
{
    boardFont = bFont;
}

void pSettings::setBoardFontColor(const QColor &bColor)
{
    boardFontColor = bColor;
}


const bool &pSettings::FirstRun()const
{
    return isFirstRun;
}

const QString &pSettings::getMainWindowTitle()const
{
    return mainWindowTitle;
}

const QRect &pSettings::getMainWindowRect()const
{
    return mainWindowRect;
}

const QColor &pSettings::getMainWindowColor() const
{
    return mainWindowColor;
}

const QFont &pSettings::getMainWindowFont()const
{
    return mainWindowFont;
}

const QFont &pSettings::getBoardFont()const
{
    return boardFont;
}

const QColor &pSettings::getBoardFontColor() const
{
    return boardFontColor;
}


