/*
 * libserial.h
 *
 *  Created on: 2014. 8. 28.
 *      Author: Byunghun Hwang <bhhwang@nsynapse.com>
 *      Desc. : Serial communication for linux
 */

#ifndef LIBSERIAL_H_
#define LIBSERIAL_H_

#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
//#include <sys/poll.h>

#define RCV_BUFFER_SIZE	1024

namespace plun {

class libserial {
public:
	libserial();
	virtual ~libserial();

	/*
	 * port open
	 */
	bool open(const char* port, int baudrate);

	/*
	 * port close
	 */
	void close();

	/*
	 * write to port
	 */
	int write(const char* data, int size);

	/*
	 * read from port
	 */
	int read(char* pdata, int size);

private:
	int _device;
	struct termios _tio;
};

} /* namespace plun */

#endif /* LIBSERIAL_H_ */
