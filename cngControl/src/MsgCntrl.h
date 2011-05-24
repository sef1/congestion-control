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

#ifndef __CNGCONTROL_MSGCNTRL_H_
#define __CNGCONTROL_MSGCNTRL_H_
#define MAC_SIZE 6

#include <omnetpp.h>
#include <Eth_pck_m.h>
#include <vector>

using namespace std;
struct tblEntry{
	unsigned int port;
	vector<unsigned short> hostNum;
};
/**
 * TODO - Generated class
 */
class MsgCntrl : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void processSelfMsg(cMessage *msg);
    virtual void processMsg(Eth_pck *msg);
    virtual void processFbMsg(Eth_pck *msg);
    int hostsNum; //Number of Hosts connected to switch
  private:
    virtual void makeTable(const char* fileName);
    unsigned short myMac[MAC_SIZE];
    tblEntry* switchTbl;
};

#endif
