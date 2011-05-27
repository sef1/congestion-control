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

network Dumbell
{
    parameters:
        int W = default(2);
        double Q_LENGTH =  default(300);//Que Length
        int Q_EQ_STABLE_PERCENT =  default(20);//Q_QE calculated from Q_LENGTH and Q_EQ_STABLE_PERSENT
        double BC_LIMIT =  default(150);
        double TIMER_PERIOD =  default(10);//mSec
        double R_AI =  default(5);//Mbps;
        double R_HAI =  default(50);//Mbps
        int FAST_RECOVERY_TH =  default(5);//stages
        double MIN_RATE =  default(10);//Mbps
        double MIN_DEC_FACTOR =  default(0.5);
        double MAX_TX =  default(1000);//Mbps
        
        
    types:
        channel Channel extends ned.DatarateChannel
        {
                datarate = 10Gbps;
                //delay = 10us;
                //ber = 1e-10;
        }
    submodules:
        host[8]: Host {
            @display("i=device/pc");
        }
        switch[2]: Switch {
            @display("i=,gold");
        }
        
    connections:

       for j=0..sizeof(switch)-1, for i=0..(sizeof(host)/2-1){
        switch[j].in[i] <-- Channel <-- host[i+j*(sizeof(host)/2)].out; 
        host[i+j*(sizeof(host)/2)].in <-- Channel <-- switch[j].out[i];
       }
       switch[0].in++ <-- Channel <-- switch[1].out++;
       switch[1].in++ <-- Channel <-- switch[0].out++;
}


