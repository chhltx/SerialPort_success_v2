#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QFont f("ZYSong18030",12); //全局调整字体
    a.setFont(f);

    MainWindow w;
    w.move(850,100);
    w.show();

    return a.exec();
}
