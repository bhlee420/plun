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

	void request(const char* component_name, RequestMsg* uri);

	bool install(const char* component_name);
	bool uninstall(const char* component_name);
};

} /* namespace plun */

#endif /* COMPONENTBROKER_H_ */
