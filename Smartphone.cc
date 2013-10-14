#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


class Smartphone : public cSimpleModule
{
  private:
    int counter;  // Note the counter here

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Smartphone);

void Smartphone::initialize()
{
    // sets the counter and puts a watch on it
    counter = 10;
    WATCH(counter);

    // sends the initial messag
    if (strcmp("tic", getName()) == 0)
    {
        EV << "Sending initial message\n";
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
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
        EV << getName() << "'s counter is " << counter << ", sending back message\n";
        send(msg, "out");
    }
}

