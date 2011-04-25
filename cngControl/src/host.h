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
class Host : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void processMsgFromLowerLayer(Eth_pck *packet);
    virtual void processSelfTimer(cMessage *msg);
    unsigned char *myMac; // will hold my mac aderess
  private: // description on what those functions do on c file
    int *randArr; // used for randoming
    vector<msgStore> msgQueue; // messages are stored here if channel is busy
    void sendMessage(Eth_pck* etherPacket,const char * gateName);
};

#endif
