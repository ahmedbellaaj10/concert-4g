#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// include CustomMessage header wich is generated at compile time
#include "CustomMessage_m.h"

class Header : public cSimpleModule
{
    protected:
        virtual CustomMessage *generateMessage(CustomMessage *msg);
        virtual void handleMessage(cMessage *msg);
};

Define_Module(Header);

/**
 * Returns the message 'as is' exchanging destination and source fields
 */
void Header::handleMessage(cMessage *msg)
{
    CustomMessage *ttmsg = check_and_cast<CustomMessage *>(msg);
    CustomMessage *newmsg = generateMessage(ttmsg);
    int numGate = msg->getArrivalGate()->getIndex();
    send(newmsg, "antennaGate$o", numGate);
}

/**
 * Sort of a factory for the custom message class
 */
CustomMessage *Header::generateMessage(CustomMessage *msg)
{
    // get source and destination
    int srcDevice = 0;
    int srcNetwork = 0;
    int destDevice = msg->getSourceIndex();
    int destNetwork = msg->getSourceNetwork();
    bool isUpstream = false;

    // writes the actual message content
    char msgname[20];
    sprintf(msgname, "down message");

    // Create message object and set source and destination field.
    CustomMessage *newmsg = new CustomMessage(msgname);
    newmsg->setSourceIndex(srcDevice);
    newmsg->setSourceNetwork(destNetwork);
    newmsg->setDestinationIndex(destDevice);
    newmsg->setDestinationNetwork(destNetwork);
    newmsg->setIsUpstream(isUpstream);

    return newmsg;
}
