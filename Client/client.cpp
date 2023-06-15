#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    //incializo el socket
    qTcpSocket = new QTcpSocket(this);
    //vinculo en data stream con el socket
    in.setDevice(qTcpSocket);
    connect(ui->btn_clse, &QAbstractButton::clicked,this, &QWidget::close);
    connect(ui->btn_request, &QAbstractButton::clicked, this, &Client::requestMessage);
    connect(qTcpSocket, &QIODevice::readyRead, this, &Client::readMessage);
}

Client::~Client()
{
    delete ui;
}

void Client::readMessage()
{
    qInfo()<<"readMessage!!!";
    in.startTransaction();
    QString receivedMessage;
    in>> receivedMessage;
    if(!in.commitTransaction())
    {
        qInfo()<<"Error en el data stream!!!";
        return;
    }
    ui->lbl_message->setText(receivedMessage);
}

void Client::requestMessage()
{
    qInfo()<<"requestMessage!!!";

    QString ipAddress = ui->le_ip->text();
    QString port = ui->le_port->text();
    //cleaning
    if(ipAddress.isEmpty())
    {
        qInfo()<<"la direccion ip es invalida";
        return;
    }
    if(port.isEmpty())
    {
        qInfo()<<"el puerto es invalido";
        return;
    }
    qInfo()<<"IP: "+ ipAddress+", puerto: "+port;
    qTcpSocket->connectToHost(ipAddress,port.toInt());

}

void Client::showError(QAbstractSocket::SocketError socket)
{

}

