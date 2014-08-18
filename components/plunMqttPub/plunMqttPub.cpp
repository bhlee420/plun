/*
 * plunMqttPub.cpp
 *
 *  Created on: 2014. 7. 4.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunMqttPub.h"
#include "../../include/plunLog.h"
#include "mqttPublisher.h"

namespace plun {

COMPONENT_INSTANCE(plunMqttPub)
COMPONENT_CREATE(plunMqttPub)
COMPONENT_DESTROY

plunMqttPub::plunMqttPub()
:iComponent(COMPONENT(plunMqttPub)), _publisher(nullptr) {

}

plunMqttPub::~plunMqttPub() {

}

bool plunMqttPub::setup()
{
	return true;
}

void plunMqttPub::run()
{
	unsigned int port = getProperty()->get("mqtt_port",1883).asUInt();
	string eth = getProperty()->get("use_ehternet", "eth0").asString();

	extern const char* getLocalAddr(const char* iface);
	_publisher = new mqttPublisher("plunMqttPub", getLocalAddr(eth.c_str()), port);
	_publisher->connect();
}

void plunMqttPub::stop()
{
	if(_publisher!=nullptr)
	{
		_publisher->disconnect();
		delete _publisher;
	}
}

void plunMqttPub::request_process(sRequestMsg* uri)
{
	_publisher->publish("comp", "from broker", 0);
}

} /* namespace plun */
