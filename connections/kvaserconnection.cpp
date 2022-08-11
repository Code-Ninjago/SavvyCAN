#include "kvaserconnection.h"

#include "canconmanager.h"


#include <QCanBus>
#include <QCanBusFrame>
#include <QDateTime>
#include <QDebug>
#include "canlib.h"

KvaserConnection::KvaserConnection() :
    CANConnection("KAPI", "KAPI_DRIVER",CANCon::KVASER_API, 1, 4000, true),
    mTimer(this)
{

}

KvaserConnection::~KvaserConnection()
{
    stop();
    //sendDebug("~SocketCANd()");
}

void KvaserConnection::piStarted()
{

}

void KvaserConnection::piStop()
{

}

void KvaserConnection::piSetBusSettings(int pBusIdx, CANBus pBus)
{

}

bool KvaserConnection::piGetBusSettings(int pBusIdx, CANBus &pBus)
{

}

void KvaserConnection::piSuspend(bool pSuspend)
{

}

bool KvaserConnection::piSendFrame(const CANFrame &)
{

}

void KvaserConnection::errorReceived(QCanBusDevice::CanBusError) const
{

}

void KvaserConnection::framesWritten(qint64 count)
{

}

void KvaserConnection::framesReceived()
{

}

void KvaserConnection::testConnection()
{

}
