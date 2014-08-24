/*
 * plunSerial.h
 *
 *  Created on: 14. 08. 24
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : plun Sync Serial Component
 */

#ifndef PLUNSERIAL_H_
#define PLUNSERIAL_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"

#include <termios.h>
#include <map>

using namespace std;

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
	map<unsigned int, int> _baudmap;

	struct termios _io_back;
	struct termios _io;



};

COMPONENT_EXPORT

} /* namespace plun */

#endif

