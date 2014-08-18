/*
 * mqttSubscriber.cpp
 *
 *  Created on: 2014. 7. 10.
 *      Author: hwang-linux
 */

#include "mqttSubscriber.h"
#include "../../include/plunLog.h"
#include <errno.h>
#include "plunMqttSub.h"

namespace plun {

static iComponent* _comp = nullptr;

mqttSubscriber::mqttSubscriber(iComponent* comp, const char* id, const char* host, int port)
:_host(host), _port(port), _message_id(0) {

	_comp = comp;
	init();
}

mqttSubscriber::~mqttSubscriber() {
	destroy();
}

void mqttSubscriber::init()
{
	mosquitto_lib_init();
	_mosquitto = mosquitto_new(NULL, true, NULL);
	if(!_mosquitto)
	{
		switch(errno)
		{
			case ENOMEM: LOG_ERROR << "Out of Memory";	break;
			case EINVAL: LOG_ERROR << "Invalid ID";		break;
		}
		mosquitto_lib_cleanup();
	}

	mosquitto_disconnect_callback_set(_mosquitto, onConnectionLost);
	mosquitto_message_callback_set(_mosquitto, onMessage);
}

void mqttSubscriber::destroy()
{
	mosquitto_destroy(_mosquitto);
	mosquitto_lib_cleanup();
}

bool mqttSubscriber::subscribe(const char* topic, int qos)
{
	bool ret = false;

	int p = mosquitto_subscribe(_mosquitto, &_message_id, topic, qos);
	if(p==MOSQ_ERR_SUCCESS)
	{
		LOG_INFO << "subscribe " << topic;
		ret = true;
	}
	else
		LOG_ERROR << mosquitto_strerror(p);

	return ret;
}

bool mqttSubscriber::connect()
{
	bool ret = false;

	mosquitto_username_pw_set(_mosquitto, NULL, NULL);
	switch(int p = mosquitto_connect(_mosquitto, _host.c_str(), _port, 60))
	{
	case MOSQ_ERR_SUCCESS:	ret = true;	LOG_INFO << "connected " << _host << ":" <<_port;	break;
	case MOSQ_ERR_INVAL: LOG_ERROR << "invalid parameters.";	break;
	case MOSQ_ERR_ERRNO:	LOG_ERROR << mosquitto_strerror(p);	break;

	}

	return ret;
}

bool mqttSubscriber::disconnect()
{
	bool ret = false;
	int rc = mosquitto_disconnect(_mosquitto);

	if(rc==MOSQ_ERR_SUCCESS) ret = true;
	else LOG_ERROR << mosquitto_strerror(rc);

	return ret;
}

void mqttSubscriber::loop()
{
	mosquitto_loop(_mosquitto, 1, 10);
}


void mqttSubscriber::onConnectionLost(mosquitto* mosq, void* obj, int rc)
{
	//dynamic_cast<plunMqttSub*>(_comp)->onConnectionLost();
}

void mqttSubscriber::onMessage(mosquitto* mosq, void* obj, const struct mosquitto_message* message)
{
	LOG_INFO << (const char*)message->payload;
	//dynamic_cast<plunMqttSub*>(_comp)->onMessage(message->topic, (const char*)message->payload, message->qos);
}

} /* namespace plun */
