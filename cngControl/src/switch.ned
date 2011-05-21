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
module Switch
{
    
    
    parameters:
      @display("i=block/switch");
      string tableFile = "switch";  
    gates:
        input in[5];
        output out[5];
    submodules:
        conP[5]: CP {
            @display("i=block/boundedqueue");
        }
        control: MsgCntrl {
            @display("i=block/dispatch");
        }
    connections:
        control.in[i] <-- in[i] for i=0..4;
        conP[i].mc <--> control.mc[i] for i=0..4;
        conP[i].out --> out[i] for i=0..4;

}
