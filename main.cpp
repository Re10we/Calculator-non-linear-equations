#include "Interfaces/mainwindow.h"

#include <QApplication>
#include "WinDark/winDark.h"
#include <QSplashScreen>
#include <QStyle>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSplashScreen *splashScreen = new QSplashScreen();
    splashScreen->setPixmap(QPixmap(":/splashScreen/Icons/SplashScreen/SplashScreen.svg"));
    splashScreen->setGeometry(QStyle::alignedRect(
           Qt::LeftToRight,
           Qt::AlignCenter,
           splashScreen->size(),
           QGuiApplication::primaryScreen()->availableGeometry()
    ));
    splashScreen->show();
    MainWindow w;
    w.setGeometry(QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                w.size(),
                QGuiApplication::primaryScreen()->availableGeometry()
     ));
    QTimer::singleShot(3000,splashScreen,SLOT(close()));
    QTimer::singleShot(2500,&w,SLOT(show()));
    //w.show();
    return a.exec();
}
