

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
