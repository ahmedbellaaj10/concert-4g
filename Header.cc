#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// include CustomPacket header wich is generated at compile time
#include "CustomPacket_m.h"

class Header : public cSimpleModule
{
    protected:
        virtual void handleMessage(cMessage *msg);
};

Define_Module(Header);

/**
 * Returns the message 'as is' exchanging destination and source fields
 */
void Header::handleMessage(cMessage *msg)
{
    CustomPacket *ttmsg = check_and_cast<CustomPacket *>(msg);
    int numGate = msg->getArrivalGate()->getIndex();
    ttmsg->setIsUpstream(false);
    send(ttmsg, "antennaGate$o", numGate);
}
