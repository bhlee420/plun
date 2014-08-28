/*
 * plunSerial.cpp
 *
 *  Created on: 14. 08. 24
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunSerial.h"
#include "../../include/plunLog.h"


namespace plun {

COMPONENT_INSTANCE(plunSerial)
COMPONENT_CREATE(plunSerial)
COMPONENT_DESTROY

plunSerial::plunSerial()
:iComponent(COMPONENT(plunSerial)), _read_task(nullptr), _write_task(nullptr), _serial(nullptr) {

	_buffer = new char[BUFFER_SIZE];
}

plunSerial::~plunSerial() {

	delete []_buffer;
}

/*
* call once when it be started
*/
void plunSerial::run()
{
	string port = getProperty()->get("port", "/dev/ttyS0").asString();
	unsigned int baudrate = getProperty()->get("baudrate", 115200).asUInt();

	_serial = new libserial();

	if(!_serial->open(port.c_str(), baudrate))
		LOG_ERROR << "Cannot open " << port;

	_read_task = Task(new TaskRegister(&plunSerial::read_task));

}

/*
* call once when it be stopped
*/
void plunSerial::stop()
{

	if(_read_task)
	{
		_read_task->interrupt();
		_read_task->join();
	}

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

/*
 * custom task loop (read from port)
 */

void plunSerial::read_task()
{
	while(1)
	{
		try
		{
			boost::mutex::scoped_lock lock(_lock);

			int read_size = 0;
			if((read_size=_serial->read(_buffer, sizeof(char)*BUFFER_SIZE))>0)
			{
				for(int i=0;i<read_size;i++)
					_receive_queue.push(_buffer[i]);
			}

			this_thread::sleep(posix_time::milliseconds(50));
		}
		catch(thread_interrupted&)
		{
			break;
		}
	}
}

void plunSerial::write_task()
{
	while(1)
	{
		try
		{
			this_thread::sleep(posix_time::milliseconds(0));
		}
		catch(thread_interrupted&)
		{
			break;
		}
	}
}

int plunSerial::write(char* data, int len)
{
	return _serial->write(data, len);
}

} /* namespace plun */

