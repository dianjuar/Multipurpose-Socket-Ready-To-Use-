#include "MultiPropuseSockets.h"

using namespace Network;


Base::DataSend::DataSend(QTcpSocket *socket)
{
    if(socket)
        sockets.push_back(socket);
}
void Base::DataSend::set_socket_DS(QTcpSocket *s)
{
    if(s)
        sockets.push_back(s);
}
void Base::DataSend::write(int index, QString s)
{
    sockets.at(index)->write( s.toUtf8() );
    socket->flush();
}
/////////////////////////////////////////////////////////
Base::DataReceiver::DataReceiver(QTcpSocket *socket)
{
    n_sockets = 0;
    sockets = new *QTcpSocket;
    if(socket)
       sockets.push_back(socket);
}

void Base::DataReceiver::set_socket_DR(QTcpSocket *s)
{
    if(s)
        sockets.push_back(s);
}

void Base::DataReceiver::readyRead()
{
    qDebug()<<("\n*********Reading**********");
    QString s( socket->readAll() );
    qDebug() << s;
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
Server::Server(QHostAddress adds, int port)
{
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()));

    if( !server->listen(adds, port) )
    {
        qDebug() << "Server couldn't start";
    }
    else
    {
       qDebug() << "Server started. Listening petitions at"<<adds<<":"<<port;
    }
}

void Server::newConnection()
{
    QTcpSocket *socket;
    socket = server->nextPendingConnection();

    set_socket_DR(socket);
    set_socket_DS(socket);

    sockets.push_back(socket);

    if(!sockets.back())
        qDebug() << "ERROR";
    else{
        /*Para saber quien respondio se debe sobre escribir QTcpSocket así llevar un registro es parte de la libreria general*/
        /*Es mejor plantear una clase que herede de estas para no reducir su enfoque general*/
        /*Usar dichas clases en comunicación*/
        connect(sockets.back(), SIGNAL(connected()),          this, SLOT(connected()));
        connect(sockets.back(), SIGNAL(disconnected()),       this, SLOT(disconnected()));
        connect(sockets.back(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
        connect(sockets.back(), SIGNAL(readyRead()),          this, SLOT(readyRead()));
    }
}
/*Debo identificar el socket que realizo esto*/
void Server::connected()
{
 //   qDebug()<<"CONECTED to -> "<<socket->localAddress()<<":"<<socket->peerPort();
     qDebug()<<"CONECTED";
}

void Server::disconnected()
{
   //  qDebug()<<"DISCONECTED to -> "<<socket->localAddress()<<":"<<socket->peerPort();
     qDebug()<<"DISCONECTED";
}

void Server::bytesWritten(qint64 bytes)
{
   // qDebug()<<"Bytes written"<<bytes<<"in"<<socket->localAddress()<<":"<<socket->peerPort();
     qDebug()<<"BYTES";
}
