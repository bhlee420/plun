/*
 * netutil.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: hwang-linux
 */

#ifndef NETUTIL_H_
#define NETUTIL_H_

namespace plun {

extern const char* getLocalAddr(const char* iface="eth0");
extern const char* getSubnetAddr(const char* iface="eth0");
extern const char* getBroadcastAddr(const char* iface="eth0");

} /* namespace plun */

#endif /* NETUTIL_H_ */
