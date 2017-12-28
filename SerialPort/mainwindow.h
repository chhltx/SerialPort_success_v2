#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QVector>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "qcustomplot.h"
#include<Qpainter>
#include<QPixmap>
#include<QMenu>
#include<QMenuBar>
#include<QAction>

extern double posX[];
extern double posY[];

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void serialPortQuery(void);
    void setMenuBar(void);

protected:
    //接口
    //实现改接口后，会在主窗口自动绘制图片
    void paintEvent(QPaintEvent*);


private slots:
    void on_openButton_clicked();
    void Read_Data();
    void serialConfig(bool);
    void openSerialPanel(void);
    void closeSerialPanel(void);
    void openDataPanel(void);
    void closeDataPanel(void);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QString str_all;
    int x = 100;
    int y = 100;
    //QPainter *painter;
    QPixmap pix;
    QPixmap pixStar;


//菜单栏定制
public:
    QMenuBar* menubar;

    QMenu* serialMenu;
    QAction* serialPanelOpen;
    QAction* serialPanelClose;

    QMenu* displayMenu;
    QAction* dataPanelOpen;
    QAction* dataPanelClose;

};

#endif // MAINWINDOW_H
