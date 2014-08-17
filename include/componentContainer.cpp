/*
 * componentContainer.cpp
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "componentContainer.h"
#include "iComponentEx.h"
#include "plunLog.h"
#include <fstream>

namespace plun {

componentContainer* componentContainer::_instance = nullptr;

bool componentContainer::install(const char* component_name)
{
	Container::iterator itr = _container.find(component_name);
	if(itr==_container.end())
	{
		if(__profile_verify(component_name))
		{
			_container.insert(Container::value_type(component_name, new iComponentEx(component_name)));
			LOG_INFO << "Installed component : " << component_name;
			return true;
		}
		else
			LOG_ERROR << "Installation Failed : " << component_name;
	}
	else
		LOG_INFO << component_name << " is already installed.";

	return false;
}


bool componentContainer::uninstall(const char* component_name)
{
	Container::iterator itr = _container.find(component_name);
	if(itr!=_container.end())
	{
		__del(itr);
		LOG_INFO << "Uninstalled component : " << component_name;
		return true;
	}
	else
		LOG_INFO << component_name << " is already uninstalled or was not installed previously.";

	return false;
}


void componentContainer::run_all()
{
	Container::iterator itr = _container.begin();
	for(;itr!=_container.end();++itr)
			itr->second->run();
}


void componentContainer::run(const char* component_name)
{
	Container::iterator itr = _container.find(component_name);
	if(itr!=_container.end())
		itr->second->run();
}


void componentContainer::stop_all()
{
	Container::iterator itr = _container.begin();
	for(;itr!=_container.end();++itr)
		itr->second->stop();
}


void componentContainer::stop(const char* component_name)
{
	Container::iterator itr = _container.find(component_name);
	if(itr!=_container.end())
		itr->second->stop();
}

void componentContainer::request(const char* component_name, RequestMsg* uri)
{
	Container::iterator itr = _container.find(component_name);
	if(itr!=_container.end())
	{
		LOG_INFO << "push request to " << component_name;
		itr->second->request(uri);
	}
}

bool componentContainer::exist(const char* component_name)
{
	Container::iterator itr = _container.find(component_name);
	if(itr!=_container.end())
		return true;

	return false;
}


void componentContainer::__del(Container::iterator &_itr)
{
	_itr->second->stop();
	delete (_itr->second);
}

bool componentContainer::__profile_verify(const char* component_name)
{
	//check profile & component exist or not
	string profile_path = "./" + string(component_name) + ".profile";
	string comp_path = "./" + string(component_name) + ".comp";

	ifstream pf_file(profile_path.c_str());
	if(!pf_file.good())	{ pf_file.close(); return false; }

	ifstream cp_file(comp_path.c_str());
	if(!cp_file.good()) { cp_file.close(); return false; }

	//read profile & syntax error checking
	stringstream buffer;
	buffer << pf_file.rdbuf();

	JValue root;
	if(!_profileReader.parse(buffer.str(), root))
	{
		LOG_ERROR << component_name << " profile read failed :" << _profileReader.getFormatedErrorMessages();
		pf_file.close();
		return false;
	}

	pf_file.close();
	cp_file.close();
	return true;
}

} /* namespace plun */
