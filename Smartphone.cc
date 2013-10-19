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

private:
    int srcDevice;
    int srcNetwork;
    int destDevice;
    int destNetwork;
    bool isUpstream;
};

Define_Module(Smartphone);

/**
 * Default constructor, does nothing but ensuring some params are null
 */
Smartphone::Smartphone(){
    timerMsg = NULL;
}

/**
 * Default destructor. Does nothing but cancelling the scheudled packet forwards
 */
Smartphone::~Smartphone(){
    cancelAndDelete(timerMsg);
}

/**
 * Called right after the class is instantiated. For some simulation reasons
 * those tings should not be called at the constructor.
 */
void Smartphone::initialize()
{
    // kickstarts the event timing
    timerMsg = new cMessage("timer");
    intervalp = &par("interval");
    scheduleAt(simTime() + intervalp->doubleValue(), timerMsg);

    // Since routing information will be static it can be initialized here
    srcDevice = getIndex();
    srcNetwork = NULL;
    destDevice = 0;
    destNetwork = 0;
    isUpstream = true;
}

/**
 * Checks if the message is the timer to send another package.
 * If it is it sends a CustomMessage, otherwise it means the 
 * one we sent has ben returned so we do nothing.
 */
void Smartphone::handleMessage(cMessage *msg)
{
    if(msg==timerMsg){
        // Sends a new message
        CustomMessage *newmsg = generateMessage();
        send(newmsg, "gate");

        // Scheudles a new event
        intervalp = &par("interval");
        scheduleAt(simTime() + intervalp->doubleValue(), timerMsg);
    }
}

/**
 * Sort of a factory for the custom message class. Uses static routing data to
 * instantiate a new CustomMessage object and returns it.
 */
CustomMessage *Smartphone::generateMessage()
{
    // writes the actual message content
    char msgname[16];
    sprintf(msgname, "upstream message");

    // Create message object and set source and destination field.
    CustomMessage *msg = new CustomMessage(msgname);
    msg->setSourceIndex(srcDevice);
    msg->setSourceNetwork(DestinationNetwork);
    msg->setDestinationIndex(destDevice);
    msg->setDestinationNetwork(destNetwork);
    msg->setIsUpstream(isUpstream);

    return msg;
}
