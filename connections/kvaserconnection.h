#ifndef KVASERCONNECTION_H
#define KVASERCONNECTION_H



#include "canconnection.h"
#include "canconmanager.h"
#include "canframemodel.h"

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

class WorkerThread : public QThread
{
    Q_OBJECT

    void run()override{
        QString result;
        count++;
        result = "Hello: " + QString::number(count);
        emit resultReady(result);
    }

signals:
    void resultReady(const QString &result);

private:
    int count = 0;
};

class KvaserConnection : public CANConnection
{
    Q_OBJECT
    QThread workerThread;

public:
    KvaserConnection();
    virtual ~KvaserConnection();
    void testRecieve();

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
    void timerSlot();

protected:
    QTimer             mTimer;

public slots:
    void handleResults(const QString &);

};

#endif // KVASERCONNECTION_H

