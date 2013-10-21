#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// include CustomPacket header wich is generated at compile time
#include "CustomPacket_m.h"

class Smartphone : public cSimpleModule
{
protected:
    cPar *intervalp;
    cMessage *timerMsg;     // message for scheudling self calls
    virtual CustomPacket *generateMessage();
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

public:
    Smartphone();
    virtual ~Smartphone();

private:
    int srcDevice;
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
    // Kickstarts the event timing
    timerMsg = new cMessage("timer");
    intervalp = &par("interval");
    scheduleAt(simTime() + intervalp->doubleValue(), timerMsg);

    // Since routing information will be final it can be initialized here
    srcDevice = getIndex();
}

/**
 * Checks if the message is the timer to send another package.
 * If it is it sends a CustomPacket, otherwise it means the
 * one we sent has ben returned so we do nothing.
 */
void Smartphone::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        // Sends a new message
        CustomPacket *newmsg = generateMessage();
        send(newmsg, "antennaGate$o");

        // Scheudles a new event
        intervalp = &par("interval");
        scheduleAt(simTime() + intervalp->doubleValue(), timerMsg);
        // TODO: measure packets/seconds
    }
    else{
        // TODO: delay measure
        delete(msg);
    }
}

/**
 * Sort of a factory for the custom message class. Uses static routing data to
 * instantiate a new CustomPacket object and returns it.
 */
CustomPacket *Smartphone::generateMessage()
{
    // Create message object and set source and direction fields.
    CustomPacket *msg = new CustomPacket(NULL);
    msg->setSourceAddress(srcDevice);
    msg->setIsUpstream(true);

    return msg;
}
