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

#ifndef __CNGCONTROL_HOST_H_
#define __CNGCONTROL_HOST_H_

#include <omnetpp.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "Eth_pck_m.h"

using namespace std;
typedef struct msgStore{
	Eth_pck* msg;
	cMessage* self;
}msgStore;

typedef enum types{general,request,reply}types;



class RP
{
public:
	/*
	 * Description:	this function initializes the RL
	 */
	RP(cDatarateChannel* channel,cModule* me);
	/*
	 * Description: the destructor for deleting stuff if needed
	 */
	virtual ~RP();
	/*
	 * those are the variables of the QCN.
	 */
	/*
	 * Description: this function handles a feedback msg
	 */
	virtual void FeedbackMsg(Eth_pck* msg);
	/*
	 * Description: this funciton calculates the stage state of the RL and increases rate if neccesery
	 */
	virtual void afterTransmit(Eth_pck* msg);
	/*
	 * Descrption: this funciton increases the rate according to information in fb
	 */
	virtual void selfIncrease();
	/*
	 * Description:	this function is called when the timer finish counting to 10ms
	 */
	virtual void timeExpired();

	double getMaxDataRate(){return MAX_DATA_RATE;};
	bool state;
	double cRate;
	double tRate;
	double TXBCount;
	int SICount;
	bool timer;
	int timerSCount;
	cModule* mySelf;
private:
	double MAX_DATA_RATE;
	double Q_EQ;
	double FB_MIN;
	double GD;
};

class Host : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void processMsgFromLowerLayer(Eth_pck *packet);
    virtual void processSelfTimer(cMessage *msg);
    unsigned char *myMac; // will hold my mac aderess
    virtual Eth_pck* generateMessage(int type,unsigned char destination);
    virtual void handleRegularMsg(Eth_pck* msg);
    virtual unsigned char decideSend();
  private: // description on what those functions do on c file
    int *randArr; // used for randoming
    vector<Eth_pck*> msgQueue; // messages are stored here if channel is busy
    void sendMessage(Eth_pck* etherPacket,const char * gateName);
    unsigned int msgIdCnt;
    // statistics counters
    unsigned long int requestMsgGenCnt;
    unsigned long int replyMsgGenCnt;
    unsigned long int generalMsgGenCnt;
    unsigned long int replyMsgRecCnt;
    RP* RL;
};

#endif
