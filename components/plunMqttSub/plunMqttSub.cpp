/*
 * plunMqttSub.cpp
 *
 *  Created on: 2014. 7. 10.
 *      Author: hwang-linux
 */

#include "plunMqttSub.h"
#include "mqttSubscriber.h"
#include "../../include/plunLog.h"

namespace plun {

COMPONENT_INSTANCE(plunMqttSub)
COMPONENT_CREATE(plunMqttSub)
COMPONENT_DESTROY

plunMqttSub::plunMqttSub()
:iComponent(COMPONENT(plunMqttSub)), _subscriber(nullptr), _thread_test(nullptr){



}

plunMqttSub::~plunMqttSub() {

}

void plunMqttSub::run()
{
	unsigned int port = getProperty()->get("mqtt_port",1883).asUInt();
	string eth = getProperty()->get("use_ethernet", "eth0").asString();
	string topic = getProperty()->get("mqtt_topic", "@host").asString();

	extern const char* getLocalAddr(const char* iface);
	_subscriber = new mqttSubscriber(this, "plunMqttSub", getLocalAddr(eth.c_str()), port);
	if(_subscriber->connect())
	{
		_subscriber->subscribe(topic.c_str(), 0);
		_thread_test = Task(new TaskRegister(&plunMqttSub::test_loop));
	}
}

void plunMqttSub::stop()
{
	if(_thread_test)
	{
		_thread_test->interrupt();
		_thread_test->join();
	}

	if(_subscriber!=nullptr)
	{
		_subscriber->disconnect();
		delete _subscriber;
	}
}

void plunMqttSub::request_process(sRequestMsg* uri)
{

}


void plunMqttSub::onMessage(char* topic, const char* msg, int qos)
{
	LOG_INFO << topic << " : " << msg;
}

void plunMqttSub::onConnectionLost()
{
	LOG_INFO << "connection losted";
}

void plunMqttSub::test_loop()
{
	while(1)
	{
		try
		{
			_subscriber->loop();	//must call this function to subscribe the topic.
			this_thread::sleep(posix_time::milliseconds(0));
		}
		catch(thread_interrupted&)
		{
			break;
		}
	}

}

} /* namespace plun */
