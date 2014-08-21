/*
 * componentContainer.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef COMPONENTCONTAINER_H_
#define COMPONENTCONTAINER_H_

#include <boost/unordered_map.hpp>
#include <string>
#include "plunTypedef.h"
#include "iComponent.h"

using namespace std;

namespace plun {

typedef unordered_map<string, iComponent*> Container;
class componentContainer {

private:
	static componentContainer* _instance;
	Container _container;
	JReader _profileReader;

public:
	static componentContainer* getContainer()
	{
		if(_instance==nullptr)
			_instance = new componentContainer;
		return _instance;
	}

	void destroy()
	{
		Container::iterator itr = _container.begin();
		for(;itr!=_container.end();++itr)
			uninstall(itr->first.c_str());
		_container.clear();

		if(_instance!=nullptr)
			delete _instance;
	}

	/*
	 * install component
	 */
	bool install(const char* component_name);
	bool install();

	/*
	 * remove component(not delete)
	 */
	bool uninstall(const char* component_name);

	/*
	 * run all installed components
	 */
	void run_all();

	/*
	 * run only one component user selected
	 */
	void run(const char* component_name);

	/*
	 * stop all installed components
	 */
	void stop_all();

	/*
	 * stop only one component user selected
	 */
	void stop(const char* compnent_name);

	/*
	 * request
	 */
	void request(const char* component_name, RequestMsg* msg);

	/*
	 * get size of container
	 */
	int getSize() const { return _container.size(); }

	/*
	 * exist check
	 */
	bool exist(const char* component_name);

	/*
	 * getting component
	 */
	iComponent* getComponent(const char* component_name);

private:
	componentContainer() {};

	void __del(Container::iterator& _itr);
	bool __profile_verify(const char* component_name);
};

} /* namespace plun */

#endif /* COMPONENTCONTAINER_H_ */
