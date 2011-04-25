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

#include "host.h"
#include "Eth_pck_m.h"
#define PREAMBLE_START 170 // 0xAA
#define PREAMBLE_END 171 // 0xAB
#define ARP_REQUEST 0
#define ARP_REPLY 1
#define CNG_TYPE 1600
#define ARP_REPLY_LENGTH 10
#define ARP_REQUEST_LENGTH 6
#define ETH_LENGTH 14
#define BROADCAST 0xFF
#define UNDEFINED -1
#define IP1 172
#define IP2 168
#define IP3 32
Define_Module(Host);

void Host::initialize()
{
		/*
		 * initializing my mac adress by using the id on the parmeters on ini file
		 */
		int myId=getIndex();
		myMac = new unsigned char[6];
		myMac[0] = 17; // 11 HEX
		myMac[1] = 17;
		myMac[2] = 17;
		myMac[3] = 17;
		myMac[4] = 17;
		myMac[5] = getIndex();
		EV << "Initialize Eth layer: "<< myMac[5] <<"\n";
		// init rand array for ip randomize, array will hold all adresses of all host but myself
		// on later stages will random one of the cells.
		int size = par("hostNum");
		size = size -1;
		randArr = new int[size];
		for (int i=0,j=0; j<size; i++)
		{
			if (i !=myId)
			{
				randArr[j]=i;
				j++;
			}
		}
		cMessage* msg = new cMessage("sendEvent");
		scheduleAt(simTime(),msg);
}

void Host::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
		processSelfTimer(msg);
	else  // message arrived from switch
		processMsgFromLowerLayer(check_and_cast<Eth_pck *>(msg));
}
void Host::processMsgFromLowerLayer(Eth_pck *packet)
{
	// checking message type - if is mac it will be handled differently
	if (packet->getLength() == CNG_TYPE)
	{
		// TODO stuff here
		delete packet;
	}
	else // regular message need to pass to check if its mine. and do stuff
	{
		bool isMine = true;
		for (unsigned int i=0;i<packet->getMacDestArraySize() && isMine;i++)
		{
			if (myMac[i] != packet->getMacDest(i))
				isMine = false;
		}
		if (isMine) // message is mine
		{
			// TODO do stuff
			delete packet;
		}
		else // message is not mine
		{
			// TODO do stuff
			delete packet;
		}
	}
}

void Host::processSelfTimer(cMessage *msg)
{
	if (!strcmp(msg->getName(),"sendEvent"))
	{
		Eth_pck * pck = new Eth_pck("sending");
		unsigned int i;
		int hostNum = par("hostNum");
		unsigned short length = par("dataLength");
		for (i=0; i<pck->getMacDestArraySize()-1;i++)
		{
			pck->setMacDest(i,myMac[i]);
		}
		pck->setMacDest(i,randArr[intuniform(0,hostNum-1)]);
		for (i=0; i<pck->getMacSrcArraySize();i++)
		{
			pck->setMacSrc(i,myMac[i]);
		}
		pck->setLength(length);
		pck->setByteLength(length);
		send(pck,"out");
		cChannel* cha= gate("out")->getTransmissionChannel();
		scheduleAt(simTime()+cha->getTransmissionFinishTime(),msg); //scheduling the event again exactly when the channel stops being busy
	}
}
