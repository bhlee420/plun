/*
 * mqttSubscriber.h
 *
 *  Created on: 2014. 7. 10.
 *      Author: hwang-linux
 */

#ifndef MQTTSUBSCRIBER_H_
#define MQTTSUBSCRIBER_H_

#include <string>
#include <mosquitto.h>
#include "../../include/iComponent.h"

using namespace std;

namespace plun {

class mqttSubscriber {
public:
	mqttSubscriber(iComponent* comp, const char* id, const char* host, int port=1883);
	virtual ~mqttSubscriber();

	bool connect();
	bool disconnect();

	bool subscribe(const char* topic, int qos);
	void loop();

private:

	void init();
	void destroy();

	static void onConnectionLost(mosquitto* mosq, void* obj, int rc);
	static void onMessage(mosquitto* mosq, void* obj, const struct mosquitto_message* message);

private:
	mosquitto* _mosquitto;
	string _host;
	unsigned int _port;
	int _message_id;
};

} /* namespace plun */

#endif /* MQTTSUBSCRIBER_H_ */
