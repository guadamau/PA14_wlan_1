/*
 * keyMSG.cc
 *
 *  Created on: 10.03.2009
 *      Author: refl
 */

#include "keyMSG.h"

keyMSG::keyMSG() {
	// TODO Auto-generated constructor stub

}

keyMSG::keyMSG(unsigned long long source, unsigned short sequenznumber){
	this->sequenznumber = sequenznumber;
	this->source = source;
 }

keyMSG::~keyMSG() {
	// TODO Auto-generated destructor stub
}
