/*
 * plunHost.h
 *
 *  Created on: 14. 08. 1
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : plun connection gate
 */

#ifndef plunHost_H_
#define plunHost_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"


#define DEFAULT_GROUP_ADDRESS	"239.0.0.1"
#define DEFAULT_GROUP_PORT		8009

namespace plun {

class plunHost : public iComponent {
public:
	plunHost();
	virtual ~plunHost();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* msg);

private:
	/*
	 * UDP response task impl.
	 */
	void response_loop();

	/*
	 * initial setting
	 */
	void setup();

	/*
	 * send to client
	 */
	bool send(const char* data, sockaddr_in* to);

private:
	/*
	 * thread for UDP(multicast) response
	 */
	Task _responseTask;

	/*
	 * UDP socket
	 */
	int _listen_socket;


	/*
	 * properties
	 */
	int _rcv_buffer_size;	//receive buffer size (default 1024)
	string _group_address;	//UDP multicast group address
	int _group_port;			//UDP multicast group port

};

COMPONENT_EXPORT

} /* namespace plun */

#endif

