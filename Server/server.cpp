#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    //fill messages
    for(int i = 1; i <= 50; i++){
        randomMessages << "Mensaje aleatorio " + QString::number(i);
    }
    qInfo() << randomMessages;
    //init server configuration
    initServer();
    // connect
    connect(ui->btn_close, &QAbstractButton::clicked, this, &QWidget::close);

    connect(qTcpServer, &QTcpServer::newConnection, this, &Server::sendRandomMessage);
}

Server::~Server()
{
    delete ui;
}

void Server::initServer()
{
    qInfo() << "init server";
    // modo listen
    qTcpServer = new QTcpServer(this);
    if (!qTcpServer->listen()){
        qInfo() << "Server error: " + qTcpServer->errorString();
        return;
    }
    // port?
    qInfo() << qTcpServer->serverPort();
    QString port = QString::number(qTcpServer->serverPort(), 10);
    qInfo() << port;
    //ui->lbl_Port->setText(port);

    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    QString ip;
    for (int i = 0; i < ipAddressList.size(); i++){
        if (ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()){
            ip = ipAddressList.at(i).toString();
            qInfo() << ip;
        }
    }

    QString info = "ip: " + ip + ", puerto: " + port;
    ui->lbl_Port->setText(info);
}

void Server::sendRandomMessage()
{
    qInfo() << "sendRandomMessage !!!";
    //ceo un canal
    QByteArray qByteArray;
    //creo un flujo
    QDataStream qDataStream(&qByteArray, QIODevice::WriteOnly);
    QString randomMessage = randomMessages[QRandomGenerator::global()->bounded(randomMessages.size())];
    qDataStream << randomMessage; //paso al flujo el mensaje

    ui->lbl_message->setText(randomMessage);//imprimo el mensaje en el server

    //delegation
    QTcpSocket *clientConnection = qTcpServer->nextPendingConnection();//aqui esta la instancia que va gestionar
    //clientConnection es un socket | el socket se asocia a un canal.
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    //cuando el cliente se desconecte ocurre el connect,
    clientConnection->write(qByteArray);
    clientConnection->disconnectFromHost();//desconecta el canal.
    //el servidor siempre esta atento a la llamada del cliente


}


