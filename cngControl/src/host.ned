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

package cngcontrol;

//
// TODO auto-generated module
//
simple Host
{
    parameters:
        @signal[RTT](type="double");
        @statistic[Rtt](title="RTT measurment"; source="RTT"; record=vector,stats,histogram; interpolationmode=linear);
        @signal[dataRate](type="double");
        @statistic[cRate](title="dataRate"; source="dataRate"; record=vector,stats; interpolationmode=sample-hold);
        @display("i=device/pc");
        int hostNum = default(8);
        volatile int dataLength;
        int destChoice = default(0);
        volatile double expThr;
        int consecutive= default(1); // how many messages in a row
        // QCN parameters for HOST
        bool USING_TIMER;
    gates:
        input in;
        output out;
}
