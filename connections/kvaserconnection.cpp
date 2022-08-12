#include "kvaserconnection.h"
#include "candemo.h"

#include "canconmanager.h"


#include <QCanBus>
#include <QCanBusFrame>
#include <QDateTime>
#include <QDebug>
#include "canlib.h"

driverData     m_channelData;
driverData    *m_DriverConfig                 = &m_channelData;
unsigned int   m_usedBaudRate                  = 0;

void InitDriver()
{
    int  i;
    canStatus  stat;

    // Initialize ChannelData.
    memset(m_channelData.channel, 0, sizeof(m_channelData.channel));
    for (i = 0; i < MAX_CHANNELS; i++) {
        m_channelData.channel[i].isOnBus      = 0;
        m_channelData.channel[i].driverMode   = canDRIVER_NORMAL;
        m_channelData.channel[i].channel      = -1;
        m_channelData.channel[i].hnd          = canINVALID_HANDLE;
        m_channelData.channel[i].txAck        = 0; // Default is TxAck off
    }
    m_channelData.channelCount = 0;

    //initialize CANlib
    canInitializeLibrary();

    //get number of present channels
    stat = canGetNumberOfChannels((int*)&m_channelData.channelCount);

    for (i = 0; (unsigned int)i < m_channelData.channelCount; i++) {
      canHandle  hnd;

      //obtain some hardware info from CANlib
      m_channelData.channel[i].channel = i;
      canGetChannelData(i, canCHANNELDATA_CHANNEL_NAME,
                        m_channelData.channel[i].name,
                        sizeof(m_channelData.channel[i].name));
      canGetChannelData(i, canCHANNELDATA_CARD_TYPE,
                        &m_channelData.channel[i].hwType,
                        sizeof(DWORD));

      //open CAN channel
      hnd = canOpenChannel(i, canOPEN_ACCEPT_VIRTUAL);
      if (hnd < 0) {
          // error
          PRINTF_ERR(("ERROR canOpenChannel() in initDriver() FAILED Err= %d. <line: %d>\n",
                      hnd, __LINE__));
        }
        else {
          m_channelData.channel[i].hnd = hnd;
          if ((stat = canIoCtl(hnd, canIOCTL_FLUSH_TX_BUFFER, NULL, NULL)) != canOK)
            PRINTF_ERR(("ERROR canIoCtl(canIOCTL_FLUSH_TX_BUFFER) FAILED, Err= %d <line: %d>\n",
                        stat, __LINE__));
        }

        m_usedBaudRate = canBITRATE_250K;

        //set the channels busparameters
        stat = canSetBusParams(hnd, m_usedBaudRate, 0, 0, 0, 0, 0);
        qDebug() << stat;
    }





}

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

bool KvaserConnection::piSendFrame(const CANFrame &pFrame)
{
    unsigned char msg[8];
    int           i;

    for(int i = 0; i < 8; i++)
    {
        msg[i] = pFrame.payload().at(i);
    }

    canWrite(m_DriverConfig->channel[0].hnd, pFrame.frameId(), msg, sizeof(pFrame.payload()), 4);

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
