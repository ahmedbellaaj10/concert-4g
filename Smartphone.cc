#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class Smartphone : public cSimpleModule
{
    private:
	int counter;
	cMessage *event;
	cMessage *messageHolder;

    protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);

    public:
	Smartphone();
	virtual ~Smartphone();
};

Define_Module(Smartphone);

/*
 *Public constructor that initializes all to null
 */
Smartphone::Smartphone()
{
    event = messageHolder = NULL;
}

/*
 *Public destructor that cancells the message on hold and emptyes 
 *the remaining one 
 */
Smartphone::~Smartphone()
{
    cancelAndDelete(event);
    delete messageHolder;
}

void Smartphone::initialize()
{
    // sets the counter and puts a watch on it
    counter = par("limit");
    WATCH(counter);

    // creates a new empty event
    event = new cMessage("event");
    // we havent received yet any message
    messageHolder = NULL; 

    // sends the initial messag
    if (par("sendMsgOnInit").boolValue() == true)
    {
	EV << "Scheduling first send to t=5.0s\n";
	messageHolder = new cMessage("messageHolder");
	scheduleAt(5.0, event);
    }
}

void Smartphone::handleMessage(cMessage *msg)
{
    // Increment counter and check value.
    counter--;
    if (counter==0)
    {
	EV << getName() << "'s counter reached zero, deleting message\n";
	delete msg;
    }
    else
    {
	if (msg==event)
	{
	    EV << "Wait period is over, sending back message\n";
	    send(messageHolder, "out");
	    messageHolder = NULL;
	}
	else
	{
	    EV << "Message arrived, starting to wait 1 sec...\n";
	    messageHolder = msg;
	    scheduleAt(simTime()+1.0, event);
	}
    }
}
