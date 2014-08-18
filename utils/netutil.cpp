/*
 * netutil.cpp
 *
 *  Created on: 2014. 7. 3.
 *      Author: hwang-linux
 */

#include "netutil.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace plun {


const char* getLocalAddr(const char* iface)
{
	struct ifreq ifr;
	int _socket = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;	//IPV4
	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
	struct in_addr local;

	if(ioctl(_socket, SIOCGIFADDR, &ifr)==0)
		local = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;

	close(_socket);

	return inet_ntoa(local);
}

const char* getSubnetAddr(const char* iface)
{
	struct ifreq ifr;
	int _socket = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;	//IPV4
	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);
	struct in_addr mask;

	if(ioctl(_socket, SIOCGIFNETMASK, &ifr)==0)
		mask = ((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr;

	close(_socket);

	return inet_ntoa(mask);
}

const char* getBroadcastAddr(const char* iface)
{

	int _socket = socket(AF_INET, SOCK_DGRAM, 0);
	struct ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

	struct in_addr local;
	struct in_addr mask;
	struct in_addr broadcast;

	if(ioctl(_socket, SIOCGIFADDR, &ifr)==0)
		local = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;

	if(ioctl(_socket, SIOCGIFNETMASK, &ifr)==0)
		mask = ((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr;

	if(ioctl(_socket, SIOCGIFBRDADDR, &ifr)==0)
		broadcast = ((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr;

	close(_socket);

	return inet_ntoa(broadcast);
}

} /* namespace plun */
