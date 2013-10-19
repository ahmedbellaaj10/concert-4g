#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// include CustomMessage header wich is generated at compile time
#include "CustomMessage_m.h"

class Smartphone : public cSimpleModule
{
    protected:
	cPar *intervalp;
	cMessage *timerMsg;     // message for scheudling self calls
	virtual CustomMessage *generateMessage();
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	virtual void forwardMessage(CustomMessage *msg);

    public:
	Smartphone();
	virtual ~Smartphone();
};

Define_Module(Smartphone);

Smartphone::Smartphone(){
    timerMsg = NULL;
}

Smartphone::~Smartphone(){
    cancelAndDelete(timerMsg);
}

void Smartphone::initialize()
{
    timerMsg = new cMessage("timer");
    intervalp = &par("interval");
    scheduleAt(simTime() + intervalp->doubleValue(), timerMsg);
}

/**
 * Checks if the message is the timer to send another package.
 * If it is it sends a CustomMessage, otherwise it means the 
 * one we sent has ben returned so we do nothing.
 */
void Smartphone::handleMessage(cMessage *msg)
    if(msg==timerMsg){
	CustomMessage *newmsg = generateMessage(); // Generates a new message
	send(newmsg, "gate"); // Sends the fresh generated message.
	simtime_t delay = par("interval"); // Gets another delay time
	scheduleAt(simTime() + intervalp->doubleValue(), timerMsg);  // Scheudles a new event.
    }
}

/**
 * sort of a constructor for the custom message class
 */
CustomMessage *Smartphone::generateMessage()
{
    // get source and destination
    int srcDevice = getIndex();
    int srcNetwork = 1;
    int destDevice = 0;
    int destNetwork = 0;
    bool isUpstream = true;

    // writes the actual message content
    char msgname[16];
    sprintf(msgname, "upstream message", src, dest);

    // Create message object and set source and destination field.
    CustomMessage *msg = new CustomMessage(msgname);
    msg->setSourceIndex(srcDevice);
    msg->setSourceNetwork(DestinationNetwork(srcNetwork);
    msg->setDestinationIndex(destDevice);
    msg->setDestinationNetwork(destNetwork);
    msg->setIsUpstream(isUpstream);

    return msg;
}
