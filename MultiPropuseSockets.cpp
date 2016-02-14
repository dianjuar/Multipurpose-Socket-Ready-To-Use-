#include "MultiPropuseSockets.h"

using namespace Network;


Base::DataSend::DataSend(QTcpSocket *socket)
{
    this->socket = socket;
}

void Base::DataSend::write(QString s)
{
    socket->write( s.toUtf8() );
    socket->flush();
}
/////////////////////////////////////////////////////////
Base::DataReceiver::DataReceiver(QTcpSocket *socket)
{
    this->socket = socket;
}

void Base::DataReceiver::readyRead()
{
    qDebug()<<"";
    qDebug()<<("*********Reading**********");
    QString s( socket->readAll() );
    qDebug()<<s;
    qDebug()<<("********Analizing***********");

    dataAnalizer(s);
}
/////////////////////////////////////////////////////////
Client::Client(QString host, int port):
    Base::Data(&socket),
    host(host),
    port(port)
{
    connected_B = false;
}

void Client::connectToHost()
{
    connect(&socket, SIGNAL(connected()), this, SLOT(connected())); //necesary to show alerts if connected.

    socket.connectToHost(host, port);

    connected_B = socket.waitForConnected(3000);

    if(connected_B)//connected
    {
        connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(&socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
        connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    else
    {
        disconnect(&socket,SIGNAL(connected()),this,SLOT(connected())); //if not connected disconnect the signal
        qDebug()<<"ERROR connecting to-> "<<host<<":"<<port;
    }
}

void Client::connected()
{
    qDebug()<<"CONECTED to -> "<<host<<":"<<port;
}

void Client::disconnected()
{
     qDebug()<<"DISCONECTED to -> "<<host<<":"<<port;
}

void Client::bytesWritten(qint64 bytes)
{
    qDebug()<<"Bytes written"<<bytes;
}
/////////////////////////////////////////////////////////////////////////
ServerSimple::ServerSimple(QHostAddress adds, int port)
{
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()) );

    if( !server->listen(adds,port) )
    {
        qDebug()<<"Server couldn't start";
    }
    else
    {
       qDebug()<<"Server started. Listening petitions at"<<adds<<":"<<port;
    }
}

void ServerSimple::newConnection()
{
    socket = server->nextPendingConnection();

    set_socket_DR(socket);
    set_socket_DS(socket);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void ServerSimple::connected()
{
    qDebug()<<"CONECTED to -> "<<socket->localAddress()<<":"<<socket->peerPort();
}

void ServerSimple::disconnected()
{
     qDebug()<<"DISCONECTED to -> "<<socket->localAddress()<<":"<<socket->peerPort();
}

void ServerSimple::bytesWritten(qint64 bytes)
{
    qDebug()<<"Bytes written"<<bytes<<"in"<<socket->localAddress()<<":"<<socket->peerPort();
}
