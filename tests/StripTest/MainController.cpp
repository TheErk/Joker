/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "MainController.h"

MainController::MainController(MainWindow * mw)
{
    _window = mw;
    loadSettings();
}

MainController::MainController()
{
}

void MainController::loadSettings()
{
    // Try to create a settings file (temp)
#if defined(Q_OS_MAC)
    _settings = new QSettings(QDir::homePath() + "/Library/Preferences/com.phonations.Joker.plist", QSettings::NativeFormat);
#elif defined(Q_OS_UNIX)
    libvlc_media_player_set_xwindow(vlcPlayer, videoWidget->winId());
#elif defined(Q_OS_WIN)
    libvlc_media_player_set_hwnd(vlcPlayer, videoWidget->winId());
#else
    qDebug() << "unknown OS please report informations";
#endif
    qDebug() << "Settings loaded fine";
    getLastFile();
}

PhString MainController::getLastFile()
{
    qDebug() << "lastfile :" << _settings->value("last_file", "").toString();
    return _settings->value("last_file").toString();
}

void MainController::setLastFile(QString filename)
{
    _settings->setValue("last_file", filename);
}

