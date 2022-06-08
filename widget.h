#ifndef WIDGET_H
#define WIDGET_H

#include <QFile>
#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>
#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    // 通信
    QUdpSocket socket;
    QHostAddress ip;
    quint16 port;

    QFile file;
    qint64 maxSize;
    qint64 sended;

    // 定时器
    QTimer timer;


private slots:
    void timeToSend();

    void selectBtn_clicked();
    void sendBtn_clicked();
    void stopBtn_clicked();
};

#endif // WIDGET_H
