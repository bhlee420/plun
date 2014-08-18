/*
 * plunMqttPub.h
 *
 *  Created on: 2014. 7. 4.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef PLUNMQTTPUB_H_
#define PLUNMQTTPUB_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"
#include <queue>

using namespace std;

namespace plun {

class mqttPublisher;
class plunMqttPub : public iComponent {
public:
	plunMqttPub();
	virtual ~plunMqttPub();

	/*
	 * interface
	 */
	virtual bool setup();
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* uri);

private:
	mqttPublisher* _publisher;
	//queue<PUB_DATA> _pub_buffer;
};

COMPONENT_EXPORT

} /* namespace plun */

#endif /* PLUNMQTTPUB_H_ */
