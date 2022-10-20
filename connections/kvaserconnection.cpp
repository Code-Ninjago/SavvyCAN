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

int TransmitMessage()
{
    unsigned int m_useID = 0xCF2102A;
    unsigned char msg[8];
    int i = 0;
    for (i = 0; i < 8; i++)
    {
        msg[i] = (unsigned char) i;
    }

    int x = canWrite(m_DriverConfig->channel[0].hnd, m_useID, msg, sizeof(msg), 4);
    return x;
}

void printDriverConfig( void )
{
  unsigned int i;

  printf("\nDriver Configuration:\n  ChannelCount=%u\n", m_DriverConfig->channelCount);
  for (i = 0; i < m_DriverConfig->channelCount; i++) {

    printf("  %s : Channel %d, isOnBus=%d, Baudrate=%u",
           m_DriverConfig->channel[i].name,
           m_DriverConfig->channel[i].channel,
           m_DriverConfig->channel[i].isOnBus,
           m_usedBaudRate);

    printf("\n    ");

    if (m_DriverConfig->channel[i].driverMode == canDRIVER_NORMAL) {
      printf("Drivermode=canDRIVER_NORMAL\n");
    } else {
      printf ("Drivermode=canDRIVER_SILENT\n");
    }
  }

  printf("\n\n");
}

void InitDriver()
{
    qDebug() << "Initialising Kvaser driver" << Qt::endl;

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
    //we only looking at the physical channel for now, this is seen to be channel 0


    //go on bus (channel 0 only)
    stat = canBusOn(m_channelData.channel[0].hnd);

    if(stat < 0)
    {
        qDebug() << "Error canBusOn(). Err " << stat << Qt::endl;
    }
    else
    {
        m_DriverConfig->channel[0].isOnBus = 1;
    }

    HANDLE tmp;
    //get CAN - eventHandles
        stat = canIoCtl(m_channelData.channel[0].hnd,
                        canIOCTL_GET_EVENTHANDLE,
                        &tmp,
                        sizeof(tmp));

    //Let's see if we can send a helloworld out over can
    TransmitMessage();
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
    InitDriver();
    if(m_DriverConfig->channel[0].isOnBus == 0)
    {
        qDebug() << "Cannot transmit message, channel 0 is off bus" << Qt::endl;
    }

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

    canStatus stat = canWrite(m_DriverConfig->channel[0].hnd, pFrame.frameId(), msg, sizeof(pFrame.payload()), 4);

    qDebug() << "kvaser can status: " << stat << Qt::endl;

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
