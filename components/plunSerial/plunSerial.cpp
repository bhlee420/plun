/*
 * plunSerial.cpp
 *
 *  Created on: 14. 08. 24
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunSerial.h"
#include "../../include/plunLog.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace plun {

COMPONENT_INSTANCE(plunSerial)
COMPONENT_CREATE(plunSerial)
COMPONENT_DESTROY

plunSerial::plunSerial()
:iComponent(COMPONENT(plunSerial)),_device(-1) {

	_baudmap[4800] = B4800;
	_baudmap[9600] = B9600;
	_baudmap[19200] = B19200;
	_baudmap[38400] = B38400;
	_baudmap[57600] = B57600;
	_baudmap[115200] = B115200;

}

plunSerial::~plunSerial() {

}

/*
* call once when it be started
*/
void plunSerial::run()
{
	string port = getProperty()->get("port", "ttyS0").asString();
	unsigned int baudrate = getProperty()->get("baudrate", 115200).asUInt();

	_device = open(port.c_str(), O_RDWR|O_NOCTTY);
	if(_device<0)
		LOG_ERROR << "Cannot open " << port;
	else
	{
		tcgetattr(_device, &_io_back);	//save settings
		bzero(&_io, sizeof(_io));
		_io.c_cflag = _baudmap[baudrate] | CRTSCTS | CS8 | CLOCAL | CREAD;
		_io.c_iflag = IGNPAR | ICRNL;
	}
}

/*
* call once when it be stopped
*/
void plunSerial::stop()
{

}

/*
* if Request Queue is not empty, request_process will be called.
*/
void plunSerial::request_process(sRequestMsg* msg)
{

}

} /* namespace plun */

