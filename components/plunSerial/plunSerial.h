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
#include "libserial.h"
#include <queue>

#define BUFFER_SIZE	512

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
	void read_task();
	void write_task();

private:
	/*
	 * function for service
	 */
	int write(char* data, int len); /* write data */
	int read(char* data, int len);	/* read data */

private:
	Task _read_task;
	Task _write_task;
	libserial* _serial;
	queue<char> _receive_queue;
	char* _buffer;

	boost::mutex _lock;



};

COMPONENT_EXPORT

} /* namespace plun */

#endif

