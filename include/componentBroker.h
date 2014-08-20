/*
 * componentBroker.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef COMPONENTBROKER_H_
#define COMPONENTBROKER_H_

#include "plunTypedef.h"

namespace plun {

class iComponent;
class componentBroker {
private:
	componentBroker() {};

private:
	static componentBroker* _instance;

public:
	static componentBroker* getBroker()
	{
		if(_instance==nullptr)
			_instance = new componentBroker;
		return _instance;
	}

	void destroy()
	{
		if(_instance!=nullptr)
			delete _instance;
	}

	/*
	 * request message to the [component_name] component
	 */
	void request(const char* component_name, RequestMsg* msg);

	/*
	 * getting component pointer for direct access
	 */
	iComponent* getComponent(const char* component_name);

	bool install(const char* component_name);
	bool uninstall(const char* component_name);
};

} /* namespace plun */

#endif /* COMPONENTBROKER_H_ */
