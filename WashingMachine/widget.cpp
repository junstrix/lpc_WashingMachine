#include "widget.h"
#include "ui_widget.h"
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <QtCore>
#include <QByteArray>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //! [0]
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);
    //make sure user can input their own port name!
    ui->portBox->setEditable(true);

    ui->baudRateBox->addItem("1200", BAUD1200);
    ui->baudRateBox->addItem("2400", BAUD2400);
    ui->baudRateBox->addItem("4800", BAUD4800);
    ui->baudRateBox->addItem("9600", BAUD9600);
    ui->baudRateBox->addItem("19200", BAUD19200);
    ui->baudRateBox->setCurrentIndex(3);

    ui->parityBox->addItem("NONE", PAR_NONE);
    ui->parityBox->addItem("ODD", PAR_ODD);
    ui->parityBox->addItem("EVEN", PAR_EVEN);

    ui->dataBitsBox->addItem("5", DATA_5);
    ui->dataBitsBox->addItem("6", DATA_6);
    ui->dataBitsBox->addItem("7", DATA_7);
    ui->dataBitsBox->addItem("8", DATA_8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->stopBitsBox->addItem("1", STOP_1);
    ui->stopBitsBox->addItem("2", STOP_2);

    ui->queryModeBox->addItem("EventDriven", QextSerialPort::EventDriven);
    ui->queryModeBox->addItem("Polling", QextSerialPort::Polling);    

    timer = new QTimer(this);
    timer->setInterval(40);

    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort(ui->portBox->currentText(), settings, QextSerialPort::EventDriven);

    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();

    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),this,SLOT(onBaudRateChanged(int)));
    connect(ui->parityBox, SIGNAL(currentIndexChanged(int)), this,SLOT(onParityChanged(int)));
    connect(ui->dataBitsBox, SIGNAL(currentIndexChanged(int)),this, SLOT(onDataBitsChanged(int)));
    connect(ui->stopBitsBox, SIGNAL(currentIndexChanged(int)), this,SLOT(onStopBitsChanged(int)));
    connect(ui->queryModeBox, SIGNAL(currentIndexChanged(int)), this,SLOT(onQueryModeChanged(int)));
    connect(ui->timeoutBox, SIGNAL(valueChanged(int)),this, SLOT(onTimeoutChanged(int)));
    connect(ui->portBox, SIGNAL(editTextChanged(QString)), this,SLOT(onPortNameChanged(QString)));
    connect(ui->openCloseButton, SIGNAL(clicked()), this,SLOT(onOpenCloseButtonClicked()));
    connect(ui->sendButton, SIGNAL(clicked()), this,SLOT(onSendButtonClicked()));
    connect(timer, SIGNAL(timeout()), this,SLOT(onReadyRead()));
    connect(port, SIGNAL(readyRead()), this,SLOT(onReadyRead()));

    connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), this,SLOT(onPortAddedOrRemoved()));
    connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), this,SLOT(onPortAddedOrRemoved()));

    setWindowTitle(tr("WashingMachine Demo"));

}

Widget::~Widget()
{
    delete ui;
    delete port;
}

void Widget::onPortNameChanged(const QString & /*name*/)
{
    if (port->isOpen()) {
        port->close();
        //ui->openCloseButton->setText("open");
//        ui->led->turnOff();
    }
}
void Widget::onBaudRateChanged(int idx)
{
    port->setBaudRate((BaudRateType)ui->baudRateBox->itemData(idx).toInt());
}

void Widget::onParityChanged(int idx)
{
    port->setParity((ParityType)ui->parityBox->itemData(idx).toInt());
}

void Widget::onDataBitsChanged(int idx)
{
    port->setDataBits((DataBitsType)ui->dataBitsBox->itemData(idx).toInt());
}

void Widget::onStopBitsChanged(int idx)
{
    port->setStopBits((StopBitsType)ui->stopBitsBox->itemData(idx).toInt());
}

void Widget::onQueryModeChanged(int idx)
{
    port->setQueryMode((QextSerialPort::QueryMode)ui->queryModeBox->itemData(idx).toInt());
}

void Widget::onTimeoutChanged(int val)
{
    port->setTimeout(val);
}
void Widget::onOpenCloseButtonClicked()
{
    if (!port->isOpen()) {
        port->setPortName(ui->portBox->currentText());
        port->open(QIODevice::ReadWrite);
    }
    else {
        port->close();
    }

    //If using polling mode, we need a QTimer
    if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
        timer->start();
    else
        timer->stop();

    //update led's status
    //ui->openCloseButton->setText("close");
    //ui->led->turnOn(port->isOpen());
}
void Widget::onSendButtonClicked()
{
    if (port->isOpen() && !ui->sendEdit->toPlainText().isEmpty())
        port->write(ui->sendEdit->toPlainText().toLatin1());
}

void Widget::onReadyRead()
{
    if (port->bytesAvailable()) {
        ui->recvEdit->moveCursor(QTextCursor::End);
        ui->recvEdit->insertPlainText(QString::fromLatin1(port->readAll()));
       // ui->status_text->
    }
}

void Widget::onPortAddedOrRemoved()
{
    QString current = ui->portBox->currentText();

    ui->portBox->blockSignals(true);
    ui->portBox->clear();
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        ui->portBox->addItem(info.portName);

    ui->portBox->setCurrentIndex(ui->portBox->findText(current));

    ui->portBox->blockSignals(false);
}
