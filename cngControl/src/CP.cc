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
	/* Denises Tests*/
	CPalg cpPoint(par("Q_eq"));
	EV << "****Denis TEST: " << cpPoint.markTable[0] << endl;
	//TODO
}

void CP::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

/*
 * Function implamentation of CPalg class
 */
CPalg::CPalg(double qeqPar)
{
	//Initialize variables
	qeq = qeqPar;
	qlen = 0;
	qntzFb = 0;
	qlenOld =0;
	fb = 0;
	markTable[0]=150;
	//markTable = {150,75,50,37.5,30,25,21.5,18.5};
}
//double CPalg::markTable[8]={150,75,50,37.5,30,25,21.5,18.5};
void CPalg::receivedFrame()
{
	fb = (qeq - qlen)-w*(qlen -qlenOld);
	if (fb < -qeq*(2*w+1))
		fb= -qeq*(2*w+1);
	else if (fb>0)
		fb=0;

}
CPalg::~CPalg()
{

}
