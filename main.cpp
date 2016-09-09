#include "mainwindow.h"

#include <QLocale>
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QLocale::setDefault(QLocale::Vietnamese);

    MainWindow w;
    w.resize(800,500);
    QSize size = w.size();
    QDesktopWidget *desktop = QApplication::desktop();
    int dw = (desktop->width() - size.width())/2;
    int dh = (desktop->height() - size.height())/2;
    w.move(dw,dh);

    w.show();

    return a.exec();
}
