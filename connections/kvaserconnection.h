#ifndef KVASERCONNECTION_H
#define KVASERCONNECTION_H

#endif // KVASERCONNECTION_H

#include <QSerialPort>
#include <QCanBusDevice>
#include <QThread>
#include <QTimer>
#include <QTcpSocket>
#include <QUdpSocket>

/*************/
#include <QDateTime>
/*************/

#include "canconnection.h"

class KvaserConnection : public CANConnection
{
    Q_OBJECT

public:
    KvaserConnection();
    virtual ~KvaserConnection();

protected:

    virtual void piStarted();
    virtual void piStop();
    virtual void piSetBusSettings(int pBusIdx, CANBus pBus);
    virtual bool piGetBusSettings(int pBusIdx, CANBus& pBus);
    virtual void piSuspend(bool pSuspend);
    virtual bool piSendFrame(const CANFrame&);

    void disconnectDevice();

private slots:
    void errorReceived(QCanBusDevice::CanBusError) const;
    void framesWritten(qint64 count);
    void framesReceived();
    void testConnection();

protected:
    QTimer             mTimer;
};



