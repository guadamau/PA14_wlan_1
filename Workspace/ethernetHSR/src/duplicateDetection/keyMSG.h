/*
 * keyMSG.h
 *
 *  Created on: 10.03.2009
 *      Author: refl
 */

#ifndef KEYMSG_H_
#define KEYMSG_H_

class keyMSG {
public:
	keyMSG();
	keyMSG(unsigned long long source, unsigned short sequenznumber);
	virtual ~keyMSG();

public:
	unsigned long long source;
	unsigned short sequenznumber;

public:

	friend bool operator < (const keyMSG& v1, const keyMSG& v2){

		if(v1.source < v2.source){
			return true;
		}else if(v1.source != v2.source){
			return false;
		}else if(v1.sequenznumber < v2.sequenznumber){
			return true;
		}else if(v1.sequenznumber != v2.sequenznumber){
			return false;
		}else{
			return false;
		}


	}

};

#endif /* keyMSG_H_ */
