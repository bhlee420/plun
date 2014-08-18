/*
 * mqttPublisher.cpp
 *
 *  Created on: 2014. 6. 20.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "mqttPublisher.h"
#include "../../include/plunLog.h"
#include <errno.h>
#include <string.h>

namespace plun {

mqttPublisher::mqttPublisher(const char* id, const char* host, int port)
:_host(host), _port(port) {

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

}

mqttPublisher::~mqttPublisher()
{
	mosquitto_destroy(_mosquitto);
	mosquitto_lib_cleanup();
}

bool mqttPublisher::connect()
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

bool mqttPublisher::disconnect()
{
	bool ret = false;

	switch(mosquitto_disconnect(_mosquitto))
	{
		case MOSQ_ERR_SUCCESS:	ret = true; break;
		case MOSQ_ERR_INVAL:		LOG_ERROR << "Invalid parameters.";	break;
		case MOSQ_ERR_NO_CONN:	LOG_ERROR << "No connection.";	break;
	}

	return ret;
}

bool mqttPublisher::publish(const char* topic, const char* msg, int QoS)
{
	int mid = 0;
	string message(msg);

	bool ret = false;

	switch(mosquitto_publish(_mosquitto, &mid, topic, strlen(msg), msg, QoS, false))
	{
		case MOSQ_ERR_SUCCESS:		LOG_INFO << "Published : " << topic << " - " << message; ret = true; break;
		case MOSQ_ERR_INVAL:			LOG_ERROR << "invalid parameters.";		break;
		case MOSQ_ERR_NOMEM:			LOG_ERROR << "Out of memory.";		break;
		case MOSQ_ERR_NO_CONN:		LOG_ERROR << "not connected to a broker.";		break;
		case MOSQ_ERR_PROTOCOL:		LOG_ERROR << "protocol error";		break;
		case MOSQ_ERR_PAYLOAD_SIZE:	LOG_ERROR << "payload size is too large.";		break;
	}

	return ret;
}



} /* namespace uosf */
