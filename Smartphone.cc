#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class Smartphone : public cSimpleModule
{

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void forwardMessage(cMessage *msg);
};

Define_Module(Smartphone);

void Smartphone::initialize()
{
    /*
     * if this is the first node it's designated to send the first message
     */
    if (getIndex()==0)
    {
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
}

/**
 * gets rid of the message if it's at the end of the route and forwards it
 * if it's not.
 */
void Smartphone::handleMessage(cMessage *msg)
{
    if (getIndex()==3)
    {
        EV << "Message " << msg << " arrived.\n";
        delete msg;
    }
    else
    {
        forwardMessage(msg);
    }
}

/**
 * forwards the message from a random output
 */
void Smartphone::forwardMessage(cMessage *msg)
{
    int n = gateSize("out"); // number of outputs
    int k = intuniform(0,n-1); // random distribution

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}
