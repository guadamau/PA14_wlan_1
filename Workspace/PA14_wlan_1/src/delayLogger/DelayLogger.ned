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

package delayLogger;


simple DelayLogger
{    
    // Number of histogram cells
    int histogramNumCells = default( 100 );
    
    // Number of observations used for range estimation
    int numFirstVals = default( 10000 );
    
    // Range extension factor (usually 1.0 .. 2)
    double rangeExtFactor = default( 1.3 );
}