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
#include "feedBack_m.h"
#define FEEDBACK 1600
#define ARP_REPLY_LENGTH 10
#define ARP_REQUEST_LENGTH 6
#define ETH_LENGTH 14
#define UNDEFINED -1
// for QCN algorithm
Define_Module(Host);
void Host::initialize()
{
		/*
		 * initializing my mac adress by using the id on the parmeters on ini file
		 */
		int myId=getIndex();
		myMac = new unsigned char[6];
		myMac[0] = 0xFF;
		myMac[1] = 0xFF;
		myMac[2] = 0xFF;
		myMac[3] = 0xFF;
		myMac[4] = 0xFF;
		myMac[5] = getIndex();
		EV << "Initialize Eth layer: "<< myMac[5] <<"\n";
		msgIdCnt=0;
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
		/*
		 * initializing statistics variables
		 */
		requestMsgGenCnt=0;
		replyMsgGenCnt=0;
		generalMsgGenCnt=0;
		replyMsgRecCnt=0;
		/*
		 * initializing variables for QCN algorithm
		 */
		RL = new RP((cDatarateChannel*)gate("out")->getTransmissionChannel());
}

void Host::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
		processSelfTimer(msg);
	else  // message arrived from switch
		processMsgFromLowerLayer(check_and_cast<Eth_pck *>(msg));
}
/*
 * Description: this function handles messages that were received from switch
 */
void Host::processMsgFromLowerLayer(Eth_pck *packet)
{
	bool isMine = true;
	for (unsigned int i=0;i<packet->getMacDestArraySize() && isMine;i++)
	{
		if (myMac[i] != packet->getMacDest(i))
			isMine = false;
	}
	if (isMine) // message is mine
	{
		if (packet->getLength() == FEEDBACK)
			RL->FeedbackMsg(packet);
		else // regular message need to pass to check if its mine. and do stuff
			handleRegularMsg(packet);
		delete packet;
	}
	else // message is not mine
	{
		// TODO do stuff
		delete packet;
	}
}
/*
 * Description: this function handles self messges,
 * 				this is the function that sends messages to out gate
 */
void Host::processSelfTimer(cMessage *msg)
{
	if (!strcmp(msg->getName(),"sendEvent"))
	{
		Eth_pck* pck;
		if (msgQueue.size() !=0)
		{
			unsigned char destination = decideSend();
			pck=generateMessage(intuniform(0,1),destination);
		}
		else // sending msg from queue
		{
			pck=msgQueue[0];
			msgQueue.erase(msgQueue.begin());
		}
		send(pck,"out");
		cChannel* cha= gate("out")->getTransmissionChannel();
		scheduleAt(simTime()+cha->getTransmissionFinishTime(),msg); //scheduling the event again exactly when the channel stops being busy
	}
}
/*
 * Description: this function generates a message to send.
 * 				Types - 0 - general, 1- Request , 2- reply
 * 				destination- the msg destination.
 */
Eth_pck* Host::generateMessage(int type,unsigned char destination)
{
	Eth_pck* pck = new Eth_pck("sending");
	unsigned int i;
	unsigned short length = par("dataLength");
	for (i=0; i<pck->getMacDestArraySize()-1;i++)
	{
		pck->setMacDest(i,myMac[i]);
	}
	pck->setMacDest(i,destination);
	for (i=0; i<pck->getMacSrcArraySize();i++)
	{
		pck->setMacSrc(i,myMac[i]);
	}
	pck->setLength(length);
	pck->setByteLength(length);
	// giving the message a number
	pck->setMsgNumber(msgIdCnt++);
			// creating the type of the message
	pck->setType(intuniform(0,1)); // 0 - General, 1- Request
	// counting statistics
	switch (type)
	{
		case general:
			generalMsgGenCnt++;
			break;
		case request:
			requestMsgGenCnt++;
			break;
		case reply:
			replyMsgGenCnt++;

	}
	return pck;
}
/*
 *	Description: this function handles regulare messages
 */
void Host::handleRegularMsg(Eth_pck* msg)
{
	if (msg->getType()==request)
	{
		Eth_pck* pck = generateMessage(reply,msg->getMacSrc(5));
		msgQueue.push_back(pck);
	}
	else if (msg->getType()==reply)
	{
		replyMsgRecCnt++;
	}
}
/*
 * Description: this function decides to which host to send data
 * 				TODO need to think more about this function
 */
unsigned char Host::decideSend()
{
	int choice = par("destChoice");
	unsigned char destination;
	switch (choice)
	{
	case 0:
		destination=(unsigned char)intuniform(0,randArr[getVectorSize()-1]);
		break;
	default:
		destination=(unsigned char)intuniform(0,randArr[getVectorSize()-1]);
		break;
	};
	return destination;
}
/*
 * Description: the constructor of RP;
 */
RP::RP(cDatarateChannel* channel)
{
	state=false;
	cRate=channel->getDatarate();
	tRate=channel->getDatarate();
	maxDataRate=channel->getDatarate();
	TXBCount=0;
	SICount=0;
	timer=false;
	timerSCount=0;
}
RP::~RP(){}

void RP::FeedbackMsg(Eth_pck* msg)
{
	FeedBack * FB = check_and_cast<FeedBack*>(msg->decapsulate());
}
void RP::afterTransmit(Eth_pck* msg)
{

}
void RP::selfIncrease()
{
}
void RP::timeExpired()
{
}
