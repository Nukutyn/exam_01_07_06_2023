#include "mytcpserver_ex.h"
#include <QDebug>
#include <QCoreApplication>


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

    while(Socket_read->bytesAvailable()>0){
        arr = Socket_read->readAll();
        quest.append(arr);
    }
    arr.clear();
    parce(quest);
}

void MyTcpServer::parce(QString arr)
{
    arr=arr.trimmed();
    QStringList zap = arr.split("&");

    if(zap[0]=="start")
    {

        zap.removeFirst();

        QString login = zap[0];
        QString roomname = zap[1];
        if(rooms.contains(roomname))
        {
            rooms[roomname]+=1;
        }
        else
        {
            rooms.insert(roomname,1);
        }
        qDebug()<<rooms;

        if(rooms[roomname]>=7)
        {
            rooms[roomname]=0;
        }

    }
    else if(zap[0]=="break")
    {
        QTcpSocket *Socket_read = (QTcpSocket*)sender();
        Socket_read->close(); // закрыть сокет
        return;
    }
    else if(zap[0]=="stats")
    {
        int k=0;
        for(int i = 0;i<user.count();i++)
        {
            k+=i;
        }
        sendToClient(QString::number(k));
    }
    else if(zap[0]=="rooms")
    {

        for(auto arr : rooms.keys())
        {
            sendToClient(arr);
        }

    }
    else if(zap[0]=="newroom")
    {
        rooms.insert(zap[1],0);

    }
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
