#ifndef MYTCPSERVER_EX_H
#define MYTCPSERVER_EX_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void sendToClient(QString);
    void slotServerRead();
    //void slotReadClient();
private:
    QTcpServer * mTcpServer;
    QVector<QTcpSocket *> user;
    int server_status;
};
#endif // MYTCPSERVER_EX_H







