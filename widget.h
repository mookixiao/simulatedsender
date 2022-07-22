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
    void updateProcessStatus();

    Ui::Widget *ui;

    // 通信
    QUdpSocket m_udpSocket;
    QHostAddress ip;
    quint16 port;

    QFile m_file;
    qint64 maxSize;
    qint64 sended;

    // 定时器
    QTimer m_timer;

public slots:
    void timeToSend();

    void processSlider_valueChanged();
    void processLineEdit_editingFinished();

    void selectBtn_clicked();
    void startBtn_clicked();
    void stopBtn_clicked();
};

#endif // WIDGET_H
