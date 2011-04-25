#ifndef __CNGCONTROL_SWITCH_H_
#define __CNGCONTROL_SWITCH_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>
#include "Eth_pck_m.h"
using namespace std;

typedef struct FilterTable{
		int gate;
		unsigned char mac[6];
		cMessage * selfEvent;
	}FilterTable;
typedef struct msgStore{
	Eth_pck* msg;
	cMessage* self;
}msgStore;
class Switch : public cSimpleModule
{

public:
	Switch();

protected:
	simtime_t agTime;//Ageing time for each table row
	simtime_t latency;//Deley for forwording message
	int tblLength;
	vector<msgStore> msgQueue;
	vector<msgStore> busyQueue; // queue for when the channel is busy
	Eth_pck *handledMsg;
	FilterTable *dataBase;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void copySrcMac(Eth_pck *src, unsigned char *dest);
    void forward(Eth_pck *msgToForward);
    void resetRow(int index);
    void initRow(int index);
    void sendMessage(Eth_pck* etherPacket,const char * gateName,int port);
};
#endif
