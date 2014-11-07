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


simple RedBoxSwitch extends HsrSwitch
{
	parameters:
        @class( RedBoxSwitch );
        int redBoxCfg = default(0); //0=Regular , 1=HSR , 2=PRP-A , 3=PRP-B
        @display("i=abstract/switch");
	gates:
		inout gateInterlink; // declare two way connections
		inout gateInterlinkExp; // declare two way connections
}
