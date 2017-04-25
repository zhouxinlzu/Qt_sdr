#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtNetwork>
#include <QAbstractSocket>

typedef enum
{
    INT_16,
    UINT_32,
    FLOAT_32,
}dataTypeEnum;
typedef enum //__attribute__((gcc_struct, packed))
{
    CMD_SET_FREQ,
    CMD_GET_IQ,
    CMD_STOP_IQ,
    CMD_SET_ATT,
    CMD_SET_IFAMP,
}cmdEnum;

typedef struct __attribute__((gcc_struct, packed))
{
    cmdEnum		en_cmd;
    qint64		i64_data;
}cmdStr;
class Interface : public QObject
{
    Q_OBJECT
public:
    explicit Interface(QObject *parent = 0);
    void connect2Server(void);
    bool connectStatus(void);
    void disConnect2Server(void);
    void send2Server(cmdEnum en_cmd,qint64 i64_data);
    void recvBufSet(intptr_t u32_addr);
    dataTypeEnum dataTypeGet(void);
    void dataIqPair(quint32 u32_iqPair);
signals:
    void startTransfer(void);
    void receivedPacket(void);
public slots:
    //void deviceLookup(const QHostInfo &device);
    void connectSuccess(void);
    void readMessage(void);
    void displayError(QAbstractSocket::SocketError);
private:
    QTcpSocket      *p_socket;
    QString         message;
    dataTypeEnum    en_iqType;
    qint32          i32_blockSize;
    quint32         u32_recvIqPair;
    float           *pf32_recvBuf;
    qint16          *pi16_recvBuf;
    quint32         *pu32_recvBuf;
};

#endif // INTERFACE_H
