//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

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
	double qlenOld;
	unsigned int qntzFb;
	int w;
	int fb;
	int generateFbFrame;
	double timeToMark;
	cModule *fatherModul;


 public:
	static double markTable[8];
	CPalg(cModule *fatherM);
	~CPalg();
	virtual FeedBack *receivedFrame(Eth_pck *incomeFrame);
	virtual int quantitize(int toQuan);
	virtual void addQlen(double len);
	virtual void popQlen(double len);
	virtual void resQlen();
};
double CPalg::markTable[8]={150,75,50,37.5,30,25,21.5,18.5};

/**
 * TODO - Generated class
 */
class CP : public cSimpleModule
{
  protected:
	CPalg *cpPoint;
	cMessage * selfEvent;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void processFbFrame(Eth_pck *msg);
    virtual void processMsgFromControl(Eth_pck *msg);
    virtual void processSelfTimer(cMessage *msg);
    virtual void msgTransmit(cMessage *selfMsg, int type);
  private:
    vector<Eth_pck*> fbMsgQueue; // Feed Back messages are stored here if channel is busy
    vector<Eth_pck*> genMsgQueue; // General messages are stored here if channel is busy
};


#endif
