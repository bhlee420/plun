/*
 * mqttPublisher.h
 *
 *  Created on: 2014. 6. 20.
 *      Author: Byunghun Hwang <bhhwang@nsynapse.com>
 */

#ifndef MQTTPUBLISHER_H_
#define MQTTPUBLISHER_H_

#include <string>
#include <mosquitto.h>


using namespace std;

namespace plun {

class mqttPublisher {
public:
	mqttPublisher(const char* id, const char* host, int port=1883);
	virtual ~mqttPublisher();

	/*
	 * Publisher Interface
	 */
	bool connect();
	bool disconnect();

	bool publish(const char* topic, const char* msg, int QoS);


private:
	mosquitto* _mosquitto;
	string _host;
	unsigned int _port;
};

} /* namespace uosf */

#endif /* MQTTPUBLISHER_H_ */
