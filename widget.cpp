#include "widget.h"
#include "ui_widget.h"

#include <iostream>

#include <QFileDialog>
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 界面
    ui->fileLineEdit->setReadOnly(true);
    ui->ipLineEdit->setText("192.168.1.102");
    ui->portLineEdit->setText("10001");
    ui->sizeLineEdit->setText("10240");
    ui->intervalLineEdit->setText("20");

    // 信号与槽
    connect(ui->selectBtn, &QPushButton::clicked, this, &Widget::selectBtn_clicked);
    connect(ui->sendBtn, &QPushButton::clicked, this, &Widget::sendBtn_clicked);
    connect(ui->stopBtn, &QPushButton::clicked, this, &Widget::stopBtn_clicked);
    connect(&timer, &QTimer::timeout, this, &Widget::timeToSend);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timeToSend()
{
    QByteArray buffer = file.read(maxSize);
    if (buffer.isEmpty()) {
        timer.stop();
    }

    socket.writeDatagram(buffer, ip, port);
    ui->progressLabel->setText(QString::number(file.pos()) + "/" + QString::number(file.size()));
}

void Widget::selectBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select file to send", "/home/ws/Video", "*.dat");
    if (file.isOpen()) {
        file.close();
    }
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);

    ui->fileLineEdit->setText(fileName);
    ui->progressLabel->setText(QString::number(file.pos()) + "/" + QString::number(file.size()));
}

void Widget::sendBtn_clicked()
{
    if (file.pos() == file.size()) {
        file.seek(0);
    }

    ip = QHostAddress(ui->ipLineEdit->text());
    port = ui->portLineEdit->text().toUShort();

    maxSize = ui->sizeLineEdit->text().toULongLong();

    timer.start(ui->intervalLineEdit->text().toInt());
}

void Widget::stopBtn_clicked()
{
    timer.stop();
}
