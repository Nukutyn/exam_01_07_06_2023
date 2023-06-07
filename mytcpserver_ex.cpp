#include "mytcpserver_ex.h"
#include <QDebug>
#include <QCoreApplication>
#include "fun_ex.h"


MyTcpServer::~MyTcpServer()
{
    //mTcpSocket->close();
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if(server_status==1){
        QTcpSocket * newCon;
        newCon = mTcpServer->nextPendingConnection();
        connect(newCon,&QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
        connect(newCon, &QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);
        user.push_back(newCon);
    }
}

void MyTcpServer::slotServerRead(){
    QTcpSocket *Socket_read = (QTcpSocket*)sender();
    QString quest,ans;
    QStringList zapros;
    QByteArray arr;
    quest.clear();
    arr.clear();
    int key;
    while(Socket_read->bytesAvailable()>0){
        arr = Socket_read->readAll();
        quest.append(arr);
    }
    arr.clear();

    sendToClient(quest.toUtf8());
}

void MyTcpServer::sendToClient(QString otvet)
{
    QTcpSocket * sentTo = (QTcpSocket*)sender();

    sentTo->write(otvet.toUtf8());

}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket * disSoc = (QTcpSocket*)sender();
    for(int i = 0; i<user.count();i++){
        if(disSoc == user.at(i)){
            user.at(i)->close();
            user.removeAt(i);
            break;
        }
    }
}
