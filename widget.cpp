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
    ui->ipLineEdit->setText("192.168.100.170");
    ui->portLineEdit->setText("10001");
    ui->intervalLineEdit->setText("5");
    ui->sizeLineEdit->setText("1280");
    ui->autoRestartCheckBox->setChecked(true);

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
    if (buffer.isEmpty()) {  // 到达文件末尾
        if (ui->autoRestartCheckBox->isChecked()) {
            file.seek(0);
            buffer = file.read(maxSize);
        }
        else {
            timer.stop();
            return;
        }
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
