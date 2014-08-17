/*
 * plunHost.cpp
 *
 *  Created on: 14. 08. 1
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunHost.h"
#include "../../include/plunLog.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

namespace plun {

COMPONENT_INSTANCE(plunHost)
COMPONENT_CREATE(plunHost)
COMPONENT_DESTROY

plunHost::plunHost()
:iComponent(COMPONENT(plunHost)),_responseTask(nullptr), _listen_socket(-1) {

}

plunHost::~plunHost() {

}

/*
* call once when it be started
*/
void plunHost::run()
{
	setup();

	_responseTask = Task(new TaskRegister(&plunHost::response_loop));
}

/*
* call once when it be stopped
*/
void plunHost::stop()
{
	if(_responseTask)
	{
		_responseTask->interrupt();
		_responseTask->join();
	}
	if(_listen_socket!=-1)
		close(_listen_socket);
}

/*
* if Request Queue is not empty, request_process will be called.
*/
void plunHost::request_process(sRequestMsg* msg)
{
	switch(msg->method)
	{
	case Method::GET:
	{
		if(msg->msg.dest_comp=="plunhost" && msg->msg.function=="knock")
		{
			string res;
			res.append("{@plunhost}");
			send(res.c_str(), &msg->src_addr);
		}
	}
	break;
	case Method::POST:
	case Method::DELETE:
	case Method::PUT:
		break;
	}
}

void plunHost::response_loop()
{
	struct sockaddr_in	_client_sockaddr;
	unsigned int _client_sockaddr_size = sizeof(_client_sockaddr);
	char* _rcv_buffer = new char[_rcv_buffer_size];

	while(1)
	{
		try
		{
			memset(_rcv_buffer, 0, sizeof(char)*_rcv_buffer_size);
			int nBytes = recvfrom(_listen_socket, _rcv_buffer, _rcv_buffer_size, 0 , (struct sockaddr*)&_client_sockaddr, (socklen_t*)&_client_sockaddr_size);

			if(nBytes!=-1)
				this->request((const char*)_rcv_buffer, &_client_sockaddr);

			this_thread::sleep(posix_time::milliseconds(1));
		}
		catch(thread_interrupted&)
		{
			delete []_rcv_buffer;
			break;
		}
	}

}

void plunHost::setup()
{
	/*
	 * getting properties from profile
	 */
	_group_address = getProperty()->get("group_address", DEFAULT_GROUP_ADDRESS).asString();
	_group_port = getProperty()->get("group_port", DEFAULT_GROUP_PORT).asUInt();
	_rcv_buffer_size = getProperty()->get("receive_buffer_size", 1024).asUInt();


	_listen_socket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in	_listen_sockaddr;
	unsigned int _rcv_sockaddr_size  = sizeof(_listen_sockaddr);

	//for reveive
	bzero(&_listen_sockaddr, sizeof(_listen_sockaddr));
	_listen_sockaddr.sin_family = AF_INET;
	_listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_listen_sockaddr.sin_port = htons(_group_port);

	struct ip_mreq	_mreq;
	_mreq.imr_multiaddr.s_addr = inet_addr(_group_address.c_str());
	_mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	//join network group
	if(setsockopt(_listen_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &_mreq, sizeof(_mreq))<0)
		LOG_ERROR << "Fail to add membership";

	//loopback = true
	unsigned int opt_loopback = 1;
	if(setsockopt(_listen_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &opt_loopback, sizeof(opt_loopback))<0)
		LOG_ERROR << "Cannot set the loopback option";

	//receive timeout 1 sec
	struct timeval timeout = {1, 0};
	if(setsockopt(_listen_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))<0)
		LOG_ERROR << "Cannot set the socket timeout";

	//bind socket for receiving data
	if(::bind(_listen_socket, (struct sockaddr *)&_listen_sockaddr, _rcv_sockaddr_size)<0)
		LOG_ERROR << "Cannot bind " << inet_ntoa(_listen_sockaddr.sin_addr) << ":" << _listen_sockaddr.sin_port;
}

bool plunHost::send(const char* data, sockaddr_in* to)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	int len = sendto(sock, data, strlen(data), 0, (struct sockaddr*)to, (socklen_t)sizeof(*to));
	close(sock);

	if(len>0)
		LOG_INFO << "Response to (" << len << ") " << inet_ntoa(to->sin_addr) << ":" << to->sin_port;
	else
		return false;

	return true;
}

} /* namespace plun */

