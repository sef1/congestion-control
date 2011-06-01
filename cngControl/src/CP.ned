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
simple CP
{
     parameters:
        @display("i=block/switch");
        @signal[QLen](type="double");
        @statistic[qlen](title="queue size"; source="QLen"; record=vector,stats,histogram; interpolationmode=linear);
        @signal[LossSg](type="double");
        @statistic[tLoss](title="Time Loss"; source="LossSg";record=stats,histogram; interpolationmode=none);
        @signal[fbCnt](type="int");
        @statistic[fbCnt](title="fb counter"; source="fbCnt";record=vector,stats,histogram; interpolationmode=none);
        volatile double Q_eq = 200.0;//Need to check this
        volatile double tToMarkRnd;
        volatile double interval;
        double MaxQlen = 300.0; //KB
        bool QCN_ON = default(true);  
    gates:
        inout mc;//Message control (mc)
        output out;
}
