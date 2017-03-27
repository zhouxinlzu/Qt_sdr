#include "interface.h"

#define INTERFACE_DBG   1

#if INTERFACE_DBG
#include <QDebug>
#endif
Interface::Interface(QObject *parent) :
    QObject(parent)
{
//    QString localHostName = QHostInfo::localHostName();
//    QHostInfo info = QHostInfo::fromName(localHostName);
//    qDebug() << "localHostName: " << localHostName << endl
//             << "ip address: " << info.addresses();
//    QHostInfo::lookupHost("box", this, SLOT(deviceLookup(QHostInfo)));

    p_socket = new QTcpSocket(this);
    connect(p_socket, &QTcpSocket::readyRead, this, &Interface::readMessage);
    connect(p_socket, &QTcpSocket::connected, this, &Interface::connectSuccess);
    connect(p_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

//void Interface::deviceLookup(const QHostInfo &device)
//{
//    if(device.error() != QHostInfo::NoError)
//    {
//        #if INTERFACE_DBG
//        qDebug() << "Lookup failed: " << device.errorString();
//        #endif
//    }
//    foreach (const QHostAddress &address, device.addresses())
//    {
//        #if INTERFACE_DBG
//        qDebug() << "Found address:" << address.toString();
//        #endif
//    }
//}

void Interface::connect2Server()
{
    u32_blockSize = 1024;
    p_socket->abort();
    p_socket->connectToHost("192.168.11.11", 60902);
}

bool Interface::connectStatus()
{
    bool b_return = false;

    if(p_socket->state() == QAbstractSocket::ConnectedState)
    {
        b_return = true;
    }

    return b_return;
}

void Interface::disConnect2Server()
{
    p_socket->abort();
}

void Interface::send2Server(cmdEnum en_cmd, qint64 i64_data)
{
    cmdStr str_cmd;

    str_cmd.en_cmd = en_cmd;
    str_cmd.i64_data = i64_data;

    QDataStream sendOut(p_socket);
    sendOut.setVersion(QDataStream::Qt_5_8);

//    qDebug() << "cmd size = " << sizeof(str_cmd);
    sendOut.writeRawData((const char *)&str_cmd, sizeof(str_cmd));
}

void Interface::recvBufSet(intptr_t u32_addr)
{
    pf32_recvBuf = (float *)u32_addr;
    qDebug() << "recv buffer addr " << pf32_recvBuf;
}

void Interface::connectSuccess()
{
    emit startTransfer();
}

void Interface::readMessage()
{
    QDataStream input(p_socket);
    input.setVersion(QDataStream::Qt_5_8);
    if(p_socket->bytesAvailable() < u32_blockSize)
    {
        return;
    }
    int readnum = 0;
    if((readnum = input.readRawData((char *)pf32_recvBuf, u32_blockSize)) < u32_blockSize)
    {
        qDebug() << "receive num ->" << readnum;
    }
    emit receivedPacket();
}

void Interface::displayError(QAbstractSocket::SocketError)
{
#if INTERFACE_DBG
    qDebug() <<"TCP ERR:" << p_socket->errorString();
#endif
}
