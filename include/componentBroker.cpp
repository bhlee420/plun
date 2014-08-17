/*
 * componentBroker.cpp
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "componentBroker.h"
#include "componentContainer.h"

namespace plun {

componentBroker* componentBroker::_instance = nullptr;

void componentBroker::request(const char* component_name, RequestMsg* uri)
{
	componentContainer::getContainer()->request(component_name, uri);
}

bool componentBroker::install(const char* component_name)
{
	return componentContainer::getContainer()->install(component_name);
}

bool componentBroker::uninstall(const char* component_name)
{
	return componentContainer::getContainer()->uninstall(component_name);
}

} /* namespace plun */
