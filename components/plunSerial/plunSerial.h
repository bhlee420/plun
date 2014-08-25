/*
 * plunSerial.h
 *
 *  Created on: 14. 08. 24
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : plun Serial Component
 *		 Depenency : libserial
 */

#ifndef PLUNSERIAL_H_
#define PLUNSERIAL_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"
#include <SerialStream.h>


using namespace std;
using namespace LibSerial;

namespace plun {

class plunSerial : public iComponent {
public:
	plunSerial();
	virtual ~plunSerial();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* msg);

private:
	int _device;
	SerialStream* _serial;



};

COMPONENT_EXPORT

} /* namespace plun */

#endif

