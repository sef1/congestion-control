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
#include <math.h>
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
		/* initializing statistics variables */
		requestMsgGenCnt=0;
		replyMsgGenCnt=0;
		generalMsgGenCnt=0;
		replyMsgRecCnt=0;
		/* initializing variables for QCN algorithm */

		RL = new RP((cDatarateChannel*)gate("out")->getTransmissionChannel(),this); //TODO check deletion

		/**** testing area for functions *****/

		/* test for handleMessage */

//		Eth_pck* test1 = new Eth_pck("testing");
//		handleMessage(test1);

		/* test for proccessMsgFromLowerLayer */

		/* test for process Self timer */

//		cMessage * test1 = new cMessage("timeExpired");
//		processSelfTimer(test1);
//		Eth_pck * test2 = generateMessage(general,decideSend());
//		msgQueue.push_back(test2);
//		cMessage * selftest = new cMessage("sendEvent");
//		processSelfTimer(selftest);

		/* test for generateMessage */
		/* test for decideSend */
		/* test for FeedbackMsg */
//		Eth_pck * testEth = new Eth_pck("send");
//		FeedBack * FB = new FeedBack("feedBack");
//		testEth->setLength(FEEDBACK);
//		FB->setFb(40);
//		FB->setQOff(-100);
//		testEth->setByteLength(55);
//		testEth->encapsulate(FB);
//		RL->SICount++;
//		RL->state=true;
//		RL->FeedbackMsg(testEth);

		/* test for afterTransmit */
		/* test for selfIncrease */
		/* test for timeExpired */
}

void Host::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
		processSelfTimer(msg);
	else  // message arrived from switch
		processMsgFromLowerLayer(check_and_cast<Eth_pck *>(msg));
}
/*
 * Description: this function handles messages that were received from CP
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
		if (msgQueue.size() ==0)
		{
			unsigned char destination = decideSend();
			pck=generateMessage(intuniform(0,1),destination);
		}
		else // sending msg from queue
		{
			pck=msgQueue[0];
			msgQueue.erase(msgQueue.begin());
		}
		cChannel* cha= gate("out")->getTransmissionChannel();
		cDatarateChannel * cha1 = (cDatarateChannel*)cha;
		cha1->setDatarate(RL->cRate);
		send(pck,"out");
		scheduleAt(simTime()+cha->getTransmissionFinishTime(),msg); //scheduling the event again exactly when the channel stops being busy
	}
	if (!strcmp(msg->getName(),"timeExpired"))
	{

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
	while (length < 200 || length > 1500)
	{
		length = par("dataLength");
	}
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
	pck->setKind(type);
	/* counting statistics*/
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
		destination=randArr[intuniform(0,getVectorSize()-2)];
		break;
	};
	return destination;
}
/*
 * Description: the constructor of RP;
 */
RP::RP(cDatarateChannel* channel,cModule* me)
{
	state=false;
	cRate=channel->getDatarate();
	tRate=channel->getDatarate();
	MAX_DATA_RATE=channel->getDatarate();
	TXBCount=0;
	SICount=0;
	timer=false;
	timerSCount=0;
	mySelf = me;
	selfTimer = new cMessage("timeExpired");
	// calculated parameters
	double percent = me->getAncestorPar("Q_EQ_STABLE_PERCENT");
	double length = me->getAncestorPar("Q_LENGTH");
	int w = me->getAncestorPar("W");
	Q_EQ = (percent*length)/100;
	FB_MIN = -Q_EQ*(2*w+1);
	GD = 1.0/(double)(2*abs(FB_MIN));

}
RP::~RP(){
	delete selfTimer;
}

void RP::FeedbackMsg(Eth_pck* msg)
{
	/* taking information from the message and deleting it when finished*/
	FeedBack * FB = check_and_cast<FeedBack*>(msg->decapsulate());
	/* info */
	int fb = FB->getFb();
	double qOff = FB->getQOff();
	double qDelta = FB->getQDelta();

	// checking if the rate limiter is inactive. need to be initialized
	if (state == false)
	{
		if (fb != 0 && qOff<0)
		{
			state= true;
			cRate = MAX_DATA_RATE;
			tRate = MAX_DATA_RATE;
			SICount = 0;
		}
		else
		{
			return;
		}
	}
	// at this stage the rate limiter is already active
	if (fb !=0 )
	{
		// using the current rate as the next target rate.
		// in the first cycle of fast recovery. fb<0 singal wuld ot reset the target rate.
		if (SICount != 0)
		{
			tRate=cRate;
			TXBCount = mySelf->getAncestorPar("BC_LIMIT");
		}
		// setting the stage counters
		SICount =0;
		timerSCount=0;

		// updating the current rate, multiplicative decrease
		double minDecFactor = mySelf->getAncestorPar("MIN_DEC_FACTOR");
		double decFactor = (1-GD*fb);
		if (decFactor < minDecFactor)
			decFactor = minDecFactor;
		cRate=cRate*decFactor;
		double minRate = mySelf->getAncestorPar("MIN_RATE");
		if (cRate< minRate)
			cRate= minRate;

		bool usingTimer= mySelf->getAncestorPar("USING_TIMER");
		if (usingTimer)
		{
			double period = mySelf->getAncestorPar("TIMER_PERIOD");
			simtime_t time = period*pow(10,-3);
			Host * temp = (Host*)mySelf;
			temp->scheduleAt(simTime()+time,selfTimer);
		}
	}
}
void RP::afterTransmit(Eth_pck* msg)
{
	// Rate limiter should be inactive if the current rate reached the maximum value
	if (cRate== MAX_DATA_RATE)
	{
		state =false;
		cRate = MAX_DATA_RATE;
		tRate = MAX_DATA_RATE;
		TXBCount= 0;
		SICount= 0;
		timer = false;
	}
	else
	{

	}
}
void RP::selfIncrease()
{
}
void RP::timeExpired()
{
}
