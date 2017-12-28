#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QRegularExpression>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //初始化主界面
    ui->setupUi(this);
    this->setWindowOpacity(1.0);//窗口整体透明度，0-1从全透明到不透明
    //this->setWindowFlags(Qt::FramelessWindowHint);//设置无边框风格

    //自绘图形
    if(!(pix.load("H:\\SerialPort_success_v1\\SerialPort\\map.jpg")))
    {
        qDebug() << tr("导入背景图片失败！");
    }

    //查询可用串口号
    serialPortQuery();
    //关闭发送按钮的使能
    qDebug() << tr("界面设定成功！");
    //初始化菜单栏
    setMenuBar();
    pixStar.load("H:\\SerialPort_success_v1\\SerialPort\\star.jpg");  //load来加载图片
}

MainWindow::~MainWindow()
{
    delete ui;
}


//读取接收到的数据
void MainWindow::Read_Data()
{
    QRegularExpression re("#*!");
    QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        QString str = tr(buf);
        QRegularExpressionMatch match  = re.match(str);
        if (match.hasMatch())
        {
            QString matched = match.captured(1);
            str_all += matched;
            str_all += '\n';

            //int index = matched.toInt() - 1; //减1
            //x.append(posX[index]);
            //y.append(posY[index]);
        }    
        ui->dataReceiver->clear();
        ui->dataReceiver->append(str_all);
    }
    x += 5;
    y += 5;
    repaint();

    buf.clear();
}

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);
        ui->BaudBox->setEnabled(false);
        ui->BitNumBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->StopBox->setEnabled(false);
        ui->openButton->setText(tr("关闭串口"));

        //连接信号槽
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
    }
}

void MainWindow::paintEvent(QPaintEvent*)
{
    //导入背景图片
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap("background.jpg"));  //绘制背景图
    painter.drawPixmap(0,40,1280,904,pix); //菜单栏占用了y坐标
    //显示当前目标位置
    //painter.drawEllipse(x,y,100,100);

    painter.drawPixmap(x,y,25,25,pixStar);  //显示五角星

}

void MainWindow::serialConfig(bool value)
{
   //打开或关闭串口设置窗口
   ui->serialPort->setVisible(value);
   ui->baudRate->setVisible(value);
   ui->bitNum->setVisible(value);
   ui->parityBit->setVisible(value);
   ui->stopBit->setVisible(value);
   ui->openButton->setVisible(value);
   //ui->serialFrame->setVisible(value);

   ui->PortBox->setVisible(value);
   ui->BaudBox->setVisible(value);
   ui->BitNumBox->setVisible(value);
   ui->ParityBox->setVisible(value);
   ui->StopBox->setVisible(value);
}

void MainWindow::openSerialPanel(void)
{
    serialConfig(true);
}

void MainWindow::closeSerialPanel(void)
{
    serialConfig(false);
}

void MainWindow::openDataPanel(void)
{
    ui->dataPanel->setVisible(true);
    ui->dataReceiver->setVisible(true);
}

void MainWindow::closeDataPanel(void)
{
    ui->dataPanel->setVisible(false);
    ui->dataReceiver->setVisible(false);
}

//查找可用的串口
void MainWindow::serialPortQuery(void)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
    //设置波特率下拉菜单默认显示第三项
    ui->BaudBox->setCurrentIndex(3);
}

void MainWindow::setMenuBar(void)
{
    //新增主菜单 （bar=栏，栅）
    menubar = new QMenuBar(this);//可以把QMenuBar理解为菜单栏的画布或背景板
    menubar->setFixedSize(maximumWidth(),40);

    /* 串口设置*///子菜单
    serialMenu = menubar->addMenu("串口");

    //打开串口设置窗口
    serialPanelOpen = new QAction("打开设置窗口", this);
    serialMenu->addAction(serialPanelOpen);
    connect(serialPanelOpen, SIGNAL(triggered()), this, SLOT(openSerialPanel()));
    //关闭串口设置窗口
    serialPanelClose = new QAction("关闭设置窗口", this);
    serialMenu->addAction(serialPanelClose);
    connect(serialPanelClose, SIGNAL(triggered()), this, SLOT(closeSerialPanel()));

    /*接收和显示*///子菜单
    displayMenu = menubar->addMenu("显示");

    //打开数据接收窗口
    dataPanelOpen = new QAction("打开接收窗口", this);
    displayMenu->addAction(dataPanelOpen);
    connect(dataPanelOpen, SIGNAL(triggered()), this, SLOT(openDataPanel()));
    //关闭数据接收窗口
    dataPanelClose = new QAction("关闭接收窗口", this);
    displayMenu->addAction(dataPanelClose);
    connect(dataPanelClose, SIGNAL(triggered()), this, SLOT(closeDataPanel()));

}
