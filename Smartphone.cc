#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// include CustomMessage header wich is generated at compile time
#include "CustomMessage_m.h"

class Smartphone : public cSimpleModule
{
    protected:
	virtual CustomMessage *generateMessage();
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	virtual void forwardMessage(CustomMessage *msg);
	virtual void updateDisplay();
	virtual void finish();

    private:
	long numSent;
	long numReceived;
        cLongHistogram hopCountStats;
        cOutVector hopCountVector;
};

Define_Module(Smartphone);

/**
 * if this is the first node it's designated to send the first message
 */
void Smartphone::initialize()
{
    // adds watches
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);

    hopCountStats.setName("hopCountStats");
    hopCountStats.setRangeAutoUpper(0, 10, 1.5);
    hopCountVector.setName("HopCount");

    if (getIndex()==0)
    {
	CustomMessage *msg = generateMessage();
	scheduleAt(0.0, msg);
    }
}

/**
 * gets rid of the message if it's at the end of the route and forwards it
 * if it's not.
 */
void Smartphone::handleMessage(cMessage *msg)
{
    CustomMessage *ttmsg = check_and_cast<CustomMessage *>(msg);

    if (ttmsg->getDestination()==getIndex())
    {
	// Message arrived.
	int hopcount = ttmsg->getHopCount();
	EV << "Message " << ttmsg << " arrived after " << hopcount << " hops.\n";
	bubble("ARRIVED, starting new one!");

	// Updating statistics
        hopCountVector.record(hopcount);
        hopCountStats.collect(hopcount);
	numReceived++;

	delete ttmsg;

	// Generate another one.
	EV << "Generating another message: ";
	CustomMessage *newmsg = generateMessage();
	EV << newmsg << endl;
	forwardMessage(newmsg);
	numSent++;

	if(ev.isGUI()) updateDisplay();
    }
    else
    {
	// We need to forward the message.
	forwardMessage(ttmsg);
    }}

/**
 * forwards the message from a random output
 */
void Smartphone::forwardMessage(CustomMessage *msg)
{
    //Increment hop count.
    msg->setHopCount(msg->getHopCount()+1);

    // Same routing as before: random gate.
    int n = gateSize("gate");
    int k = intuniform(0,n-1);

    EV << "Forwarding message " << msg << " on gate[" << k << "]\n";
    send(msg, "gate$o", k);
}

/**
 * sort of a constructor for the custom message class
 */
CustomMessage *Smartphone::generateMessage()
{
    // get source and destination
    int src = getIndex();   // our module index
    int n = size();      // module vector size
    int dest = intuniform(0,n-2);
    if (dest>=src) dest++;

    // writes the actual message content
    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    // Create message object and set source and destination field.
    CustomMessage *msg = new CustomMessage(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

/**
 * updates labels on the simulation enviroment to
 * see the counters
 */
void Smartphone::updateDisplay()
{
    char buf[40];
    sprintf(buf, "rcvd: %ld sent: %ld", numReceived, numSent);
    getDisplayString().setTagArg("t",0,buf);
}

void Smartphone::finish()
{
    //This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:     " << numSent << endl;
    EV << "Received: " << numReceived << endl;
    EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
    EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
    EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
    EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;

    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);

    hopCountStats.recordAs("hop count");
}
