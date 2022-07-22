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
    ui->ipLineEdit->setText("192.168.100.171");
    ui->portLineEdit->setText("10001");
    ui->intervalLineEdit->setText("5");
    ui->sizeLineEdit->setText("1280");
    ui->autoRestartCheckBox->setChecked(true);

    // 信号与槽
    connect(ui->selectBtn, &QPushButton::clicked, this, &Widget::selectBtn_clicked);
    connect(ui->startBtn, &QPushButton::clicked, this, &Widget::startBtn_clicked);
    connect(ui->stopBtn, &QPushButton::clicked, this, &Widget::stopBtn_clicked);

    connect(&m_timer, &QTimer::timeout, this, &Widget::timeToSend);

    connect(ui->processSlider, &QSlider::valueChanged, this, &Widget::processSlider_valueChanged);
    connect(ui->processLineEdit, &QLineEdit::editingFinished, this, &Widget::processLineEdit_editingFinished);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateProcessStatus()
{
    ui->processSlider->setValue(m_file.pos());
    ui->processLineEdit->setText(QString::number(m_file.pos()));

    float percent = (float)m_file.pos() / m_file.size() * 100;
    ui->processPercentLabel->setText(QString::number(percent, 'f', 2) + "%");
}

void Widget::timeToSend()
{
    QByteArray buffer = m_file.read(maxSize);
    if (buffer.isEmpty()) {  // 到达文件末尾
        if (ui->autoRestartCheckBox->isChecked()) {
            m_file.seek(0);
            buffer = m_file.read(maxSize);
        }
        else {
            m_timer.stop();
            return;
        }
    }

    m_udpSocket.writeDatagram(buffer, ip, port);
    updateProcessStatus();
}

void Widget::selectBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select file to send", "/home/auto/Video", "*.dat");
    if (m_file.isOpen()) {
        m_file.close();
    }
    m_file.setFileName(fileName);
    m_file.open(QIODevice::ReadOnly);

    ui->fileLineEdit->setText(fileName);
    ui->processSlider->setMinimum(0);
    ui->processSlider->setMaximum(m_file.size());
    ui->processLabel->setText("/" + QString::number(m_file.size()));

    updateProcessStatus();
}

void Widget::startBtn_clicked()
{
    if (m_file.pos() == m_file.size()) {
        m_file.seek(0);
    }

    ip = QHostAddress(ui->ipLineEdit->text());
    port = ui->portLineEdit->text().toUShort();

    maxSize = ui->sizeLineEdit->text().toULongLong();

    m_timer.start(ui->intervalLineEdit->text().toInt());
}

void Widget::stopBtn_clicked()
{
    m_timer.stop();
}

void Widget::processSlider_valueChanged()
{
    m_file.seek(ui->processSlider->value());
    updateProcessStatus();
}

void Widget::processLineEdit_editingFinished()
{
    int pos = ui->processLineEdit->text().toInt();
    if (pos >= 0 && pos <=  m_file.size()) {
        m_file.seek(pos);
        updateProcessStatus();
    }
}
