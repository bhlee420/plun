/*
 * plunSerial.cpp
 *
 *  Created on: 14. 08. 24
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunSerial.h"
#include "../../include/plunLog.h"

using namespace LibSerial;

namespace plun {

COMPONENT_INSTANCE(plunSerial)
COMPONENT_CREATE(plunSerial)
COMPONENT_DESTROY

plunSerial::plunSerial()
:iComponent(COMPONENT(plunSerial)),_serial(nullptr) {


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

	if(_serial==nullptr)
	{
		_serial = new SerialStream();
		_serial->Open(port);
		switch(baudrate)
		{
		case 1200:		_serial->SetBaudRate(SerialStreamBuf::BAUD_1200); break;
		case 1800:		_serial->SetBaudRate(SerialStreamBuf::BAUD_1800); break;
		case 2400:		_serial->SetBaudRate(SerialStreamBuf::BAUD_2400); break;
		case 4800:		_serial->SetBaudRate(SerialStreamBuf::BAUD_4800); break;
		case 9600:		_serial->SetBaudRate(SerialStreamBuf::BAUD_9600); break;
		case 19200:	_serial->SetBaudRate(SerialStreamBuf::BAUD_19200); break;
		case 38400:	_serial->SetBaudRate(SerialStreamBuf::BAUD_38400); break;
		case 57600:	_serial->SetBaudRate(SerialStreamBuf::BAUD_57600); break;
		case 115200:	_serial->SetBaudRate(SerialStreamBuf::BAUD_115200); break;
		}
		_serial->SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	}
}

/*
* call once when it be stopped
*/
void plunSerial::stop()
{
	if(_serial)
	{
		delete _serial;
		_serial = nullptr;
	}
}

/*
* if Request Queue is not empty, request_process will be called.
*/
void plunSerial::request_process(sRequestMsg* msg)
{

}

} /* namespace plun */

