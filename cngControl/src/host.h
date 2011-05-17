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
typedef struct RL{
	bool state;
	double cRate;
	double tRate;
	double TXBCount;
	int SICount;
	bool timer;
	int timerSCount;
}RL;
typedef enum types{general,request,reply}types;
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
    virtual void handleFeedbackMsg(Eth_pck* msg);
    virtual unsigned char decideSend();
    virtual void afterSending(Eth_pck* msg);
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

    // Variables for QCN algorithm
    RL rateLimiter;
};

#endif
