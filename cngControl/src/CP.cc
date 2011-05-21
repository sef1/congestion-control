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

#include "CP.h"

Define_Module(CP);

void CP::initialize()
{
	cpPoint = new CPalg(par("Q_eq"),(cModule*)this);
	Eth_pck *test = new Eth_pck();
	test->setByteLength(2024);
	cpPoint->receivedFrame(test);
	//EV << "****Denis TEST: " << cpPoint.markTable[0] << endl;
	//TODO
}

void CP::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
	if (check_and_cast<Eth_pck *>(msg)->getType()==0)
		processFbFrame(msg);//Feedback frame immediately forwarded to the channel without queuing
	else  // message arrived from MsgControl
		processMsgFromControl(check_and_cast<Eth_pck *>(msg));
}
/*
 * This function immediately pass msg that shoud be Feed Back frame to the channel
 */
void CP::processFbFrame(cMessage *msg)
{

}
/*
 * This function take care to all other messages that need to send
 */
void CP::processMsgFromControl(Eth_pck *msg)
{

}
/*
 * Function implamentation of CPalg class
 */
CPalg::CPalg(double qeqPar, cModule* fatherM)
{
	fatherModul = fatherM;
	//Initialize variables
	qeq = qeqPar;
	qlen = 250;//TODO chenge this
	qntzFb = 0;
	qlenOld =0;
	fb = 0;
	w = 2;
	timeToMark = markTable[0];
}
/*
 * 6 bit quantize get 6 MSB from param toQuan
 */
int CPalg::quantitize(int toQuan)
{
	unsigned int temp = 1;
	unsigned int qntzFb = 0;
	if (toQuan >= 0)
		return 0;
	qntzFb = -1*toQuan;
	temp = (temp << 31);
	while(!(qntzFb & temp))
		qntzFb = (qntzFb << 1);
	temp = (temp >> 5);
	return qntzFb/temp;
}
void CPalg::forward(Eth_pck *fbMsg)
{
	//TODO complete this
	//need to check that channel is busy
	//blablabla...
}
//double CPalg::markTable[8]={150,75,50,37.5,30,25,21.5,18.5};
void CPalg::receivedFrame(Eth_pck *incomeFrame)
{
	double nextPeriod;
	double rnd;

	fb = (qeq - qlen)-w*(qlen -qlenOld);
	if (fb < -qeq*(2*w+1))
		fb= -qeq*(2*w+1);
	else if (fb>0)
		fb=0;

	qntzFb = quantitize(fb);//TODO need to check this function

	//sampeling probability is a function of FB
	generateFbFrame = 0;

	timeToMark -=incomeFrame->getByteLength()/1000;//length in KB

	if (timeToMark > 0)//TODO must be <
	{
		//generate a feedback Frame if fb is negative (qntzFb = 0 if fb =0 or positive)
		if (qntzFb > 0)
			generateFbFrame = 1;
		//update qlenOld
		qlenOld = qlen;
		nextPeriod = markTable[qntzFb/8];
		//tToMarkRnd parameter that will return rand number defined in ini file
		rnd = fatherModul->par("tToMarkRnd");
		timeToMark=rnd*nextPeriod;
	}


	if (generateFbFrame)
	{
		//TODO generate FeedBack Frame
//		Eth_pck *packet;
//		forward(packet);
	}
}
CPalg::~CPalg()
{

}
