#ifndef NETWORKING_H
#define NETWORKING_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>
#include <vector>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QThread>

namespace Network
{
    namespace Base
    {
        class DataReceiver: public QObject
        {
            Q_OBJECT
                private:
                    QTcpSocket socket;
                    virtual void dataAnalizer(QString msj){}

                public slots:
                    void readyRead();

                public:
                    DataReceiver(QTcpSocket socket = NULL);
                    //setters
                    void set_socket_DR(QTcpSocket socket);
        };
        ////////////////////////////////////////////
        class DataSend
        {
            private:
                 QTcpSocket socket;

            public:
                DataSend(QTcpSocket socket = NULL);
                //setters
                void set_socket_DS(QTcpSocket socket);
                void write(QString s);
        };
        /*Creating this will allow the creation arrays of clients or servers*/
        class Data: public Network::Base::DataReceiver, public Network::Base::DataSend {
          public:
            Data(QTcpSocket *socket) : DataReceiver(socket), DataSend(socket){}
            Data(){}
        };
    }

    ////////////////////////////////////////////
    class Server: public QTcpServer
    {
        Q_OBJECT
            public:
                explicit Server(QHostAddress adds, int port, QObject *parent = 0);
                void     startServer();

            protected:
                void incomingConnection(int socket_descriptor);

            signals:

            public slots:

    };

    ////////////////////////////////////////////

    class connection: public QThread, public Network::Base::Data
    {
        Q_OBJECT
            public:
                explicit connection(int socket_id, QObject *parent = 0);
                void     run();
            private:
                int         socket_descriptor;
                QTcpSocket *socket;
            signals:
                void error(QTcpSocket::SocketError serror);

            protected:
                void incomingConnection(int socket_descriptor);

            public slots:
                void disconnected();

    };

    ////////////////////////////////////////////
    class Client: public Network::Base::Data
    {
        Q_OBJECT
    private:
       QTcpSocket socket;

    protected:
       QString host;
       int     port;
       bool    connected_B;

    public:
       Client(QString host, int port);
       void connectToHost();

       //getters
       bool    isConnected()   { return connected_B; }
       QString get_host()   { return host; }
       int     get_port()       { return port; }

    signals:

    public slots:
       void	connected();
       void	disconnected();
       void	bytesWritten(qint64 bytes);
    };

}

#endif // NETWORKING_H
