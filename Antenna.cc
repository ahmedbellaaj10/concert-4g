#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

#include "CustomPacket_m.h"


/**
 * Really simple class than only forwards messages from one net to another
 * depending on the upstream boolean.
 * At some point we should implement some kindof data limitation but by the
 * time we only forward messages, no delay considered.
 */
class Antenna : public cSimpleModule
{
    protected:
        virtual void handleMessage(cMessage *msg);
};

Define_Module(Antenna);

/**
 * Forwards the message in the right direction
 */
void Antenna::handleMessage(cMessage *msg)
{
    CustomPacket *ttmsg = check_and_cast<CustomPacket *>(msg);

    // If the direction is smartphone-->header
    if (ttmsg->getIsUpstream())
    {
        int numGate = ttmsg->getArrivalGate()->getIndex();
        ttmsg->setSourceAddress(numGate);
        send(ttmsg,"headerGate$o");
    }
    // If the direction is header-->smartphone
    else
    {
        int numSmartphone = ttmsg->getSourceAddress();
        send(ttmsg,"deviceGate$o", numSmartphone);
    }
}
