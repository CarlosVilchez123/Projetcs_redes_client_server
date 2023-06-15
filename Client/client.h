#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void readMessage();
    void requestMessage();
    void showError(QAbstractSocket::SocketError socket);
private:
    Ui::Client *ui;
    QTcpSocket *qTcpSocket = nullptr;
  //  QString receivedMessage;
    QDataStream in;
};
#endif // CLIENT_H
