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

#include "MsgCntrl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
Define_Module(MsgCntrl);

void MsgCntrl::initialize()
{
    //Getting index of parent module for MAC address
	for (int i = 0; i < MAC_SIZE; i++){
		if (i == 4)
			myMac[i]=getParentModule()->getIndex();
		else
			myMac[i]=0xFF;
	}
	string temp=getParentModule()->par("tableFile");
	char* fileName= new char[10];
	sprintf(fileName,"%s%d",temp.c_str(),myMac[4]);
	cGate *g=gate("in",0);
	unsigned int hostsNum = g->getVectorSize();
	switchTbl = new tblEntry[hostsNum];
	makeTable(fileName, hostsNum);
//	EV << fileName << endl;
//	FILE* f1=fopen(fileName,"rt");
//	fscanf(f1,"%s",temp2);
//	EV << temp2 << endl;

//	cGate* inGate = gate("in",0);
//	inGate->getNextGate();
//	for (int i = 0; i < SWITCH_PORT_NUM; i++)
//	{
//		switchTbl[i].gateName=getGateNames().at(i);
//	}
	//EV << getGateNames().at(1) << endl;

}

/*
 * Description: This Function makes switch table from file
 */
void MsgCntrl::makeTable(const char* fileName, const int portNum)
{
	FILE* fStr = fopen(fileName,"r");
	int tPort;
	char* tHost = new char[4];//max 255 host for network
	for (int i = 0; i < portNum; i++)
	{
		fscanf(fStr,"%d",&tPort);
		switchTbl[i].port = tPort;
		fscanf(fStr,"%s",tHost);
		while(strcmp(tHost,"$"))
		{
			switchTbl[i].hostNum.push_back(atoi(tHost));
			fscanf(fStr,"%s",tHost);
		}
	}
	for(int j=0;j<4;j++){EV << "host: " << switchTbl[4].hostNum[j] << endl;}
	fclose(fStr);
	delete tHost;

}

void MsgCntrl::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}
