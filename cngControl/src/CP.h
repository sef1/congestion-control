
//TODO counter for loss packets

//TODO create finish functions in order to delete all undisposed selfmessages and all messages on queues


//TODO create PowerPoint show

#ifndef __CNGCONTROL_CP_H_
#define __CNGCONTROL_CP_H_

#include <omnetpp.h>
#include "Eth_pck_m.h"
#include "feedBack_m.h"
#include <string.h>
#include <stdio.h>
#include <vector>
using namespace std;

/*
 * This class define Conjection Point variables and function
 */
class CPalg
{
 protected:
	double qeq;
	double qlen;
	double tLoss;//TODO
	double qlenOld;
	unsigned int qntzFb;
	int w;
	int fb;
	int generateFbFrame;
	double timeToMark;
	cModule *fatherModul;


 public:

	//statistic variables and graphs
	double maxLen;

	//cOutVector qLenStat;

	static double markTable[8];
	CPalg(cModule *fatherM);
	~CPalg();
	virtual Eth_pck *receivedFrame(Eth_pck *incomeFrame);
	virtual unsigned int quantitize(int toQuan);
	virtual bool addQlen(double len);
	virtual void popQlen(double len);
	virtual void resQlen();
};
double CPalg::markTable[8]={150.0,75.0,50.0,37.5,30.0,25.0,21.5,18.5};

/**
 * TODO - Generated class
 */
class CP : public cSimpleModule
{
public:
	/* statistics variables */
	simsignal_t qlenSig;

	simsignal_t lossSig;

	simsignal_t fbSig;
	double lastTime;
	double interval;
	int fbCnt;
protected:
	CPalg *cpPoint;
	cMessage * selfEvent;
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	virtual void finish();
	virtual void processMsg(Eth_pck *msg);
	virtual void processSelfTimer(cMessage *msg);
	virtual void msgTransmit(cMessage *selfMsg, int type);
private:
	vector<Eth_pck*> fbMsgQueue; // Feed Back messages are stored here if channel is busy
	vector<Eth_pck*> genMsgQueue; // General messages are stored here if channel is busy


};


#endif
