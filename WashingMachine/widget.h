#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qextserialport.h"

namespace Ui {
    class Widget;
}
class QTimer;
class QextSerialPort;
class QextSerialEnumerator;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
//protected:
//    void changeEvent(QEvent *e);

private:
    Ui::Widget *ui;
    QTimer *timer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;

private Q_SLOTS:
    void onPortNameChanged(const QString &name);
    void onBaudRateChanged(int idx);
    void onParityChanged(int idx);
    void onDataBitsChanged(int idx);
    void onStopBitsChanged(int idx);
    void onQueryModeChanged(int idx);
    void onTimeoutChanged(int val);
    void onOpenCloseButtonClicked();
    void onSendButtonClicked();
    void onReadyRead();

    void onPortAddedOrRemoved();
};

#endif // WIDGET_H
