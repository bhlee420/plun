/*
 * plunMqttSub.h
 *
 *  Created on: 2014. 7. 10.
 *      Author: hwang-linux
 */

#ifndef PLUNMQTTSUB_H_
#define PLUNMQTTSUB_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"


namespace plun {

class mqttSubscriber;
class plunMqttSub : public iComponent {
public:
	plunMqttSub();
	virtual ~plunMqttSub();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* uri);

	void onMessage(char* topic, const char* msg, int qos);
	void onConnectionLost();

private:
	mqttSubscriber* _subscriber;

private:
	void test_loop();

private:
	Task _thread_test;
};

COMPONENT_EXPORT

} /* namespace plun */

#endif /* PLUNMQTTSUB_H_ */
