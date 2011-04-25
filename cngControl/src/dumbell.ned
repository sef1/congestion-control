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
       types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 100Mbps;
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

        switch[0].in++ <-- Channel <-- host[0].out;
        host[0].in <-- Channel <-- switch[0].out++;

        switch[0].in++ <-- Channel <-- host[1].out;
        host[1].in <-- Channel <-- switch[0].out++;

        switch[0].in++ <-- Channel <-- host[2].out;
        host[2].in <-- Channel <-- switch[0].out++;

        switch[0].in++ <-- Channel <-- host[3].out;
        host[3].in <-- Channel <-- switch[0].out++;
        
        switch[0].in++ <-- Channel <-- switch[1].out++;
        switch[1].in++ <-- Channel <-- switch[0].out++;
        
     //   -------------------------------------------------
        switch[1].in++ <-- Channel <-- host[4].out;
        host[4].in <-- Channel <-- switch[1].out++;

        switch[1].in++ <-- Channel <-- host[5].out;
        host[5].in <-- Channel <-- switch[1].out++;

        switch[1].in++ <-- Channel <-- host[6].out;
        host[6].in <-- Channel <-- switch[1].out++;

        switch[1].in++ <-- Channel <-- host[7].out;
        host[7].in <-- Channel <-- switch[1].out++;
}
