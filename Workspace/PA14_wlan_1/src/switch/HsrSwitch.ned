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

package switch;

simple HsrSwitch
{
    parameters:
        string macAddress;
        string schedulerMode = default("FCFS");
        int timeslotPhaseSize = default(0);
        int ringID; //ring number
        int framebytelimitPerSecond = default(-1);
		@display("i=abstract/switch");
	gates:
        inout gateA; // declare two way connections
        inout gateAExp; // declare two way connections
        inout gateB; // declare two way connections
        inout gateBExp; // declare two way connections
        inout gateCPU; // declare two way connections
        inout gateCPUExp; // declare two way connections
}
