/*
 * iComponentEx.cpp
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "iComponentEx.h"
#include <dlfcn.h>
#include <fstream>

namespace plun {

iComponentEx::iComponentEx(const char* component_name)
:iComponent(component_name), _handle(nullptr), _thread(nullptr){

	string component_path = "./"+string(component_name)+".comp";

	_handle = dlopen(component_path.c_str(), RTLD_LAZY|RTLD_GLOBAL);

	if(!_handle)
		LOG_ERROR << dlerror();
	else
	{
		createComponent create = (createComponent)dlsym(_handle, "create");
		if(dlerror()!=NULL)
			LOG_ERROR << dlerror();
		else
		{
			_pInstance = create();
			if(_pInstance!=nullptr)
			{
				_pInstance->init();
				_pInstance->loadProfile(component_name);
			}
		}
	}

}

iComponentEx::~iComponentEx() {

	if(_pInstance!=nullptr)
	{
		destroyComponent destroy = (destroyComponent)dlsym(_handle, "destroy");

		if(dlerror()!=NULL)
			LOG_ERROR << dlerror();
		else
		{
			if(_thread!=nullptr)
			{
				_thread->interrupt();
				_thread->join();
			}

			if(_pProfileFile!=nullptr)
				delete _pProfileFile;

			if(_pInBox!=nullptr)
				delete _pInBox;

			destroy();
			_pInstance = nullptr;
		}
	}

	if(_handle)
		dlclose(_handle);
}

void iComponentEx::run()
{
	if(_pInstance!=nullptr)
	{
		_pInstance->run();
		if(_thread==nullptr)
			_thread = Task(new TaskRegister(&iComponentEx::comp_inner_loop));
	}
}

void iComponentEx::stop()
{
	if(_thread!=nullptr)
	{
		_thread->interrupt();
		_thread->join();
		_thread = nullptr;
	}

	if(_pInstance!=nullptr)
		_pInstance->stop();
}

void iComponentEx::comp_inner_loop()
{
	if(_pInstance!=nullptr)
	{
		while(1)
		{
			try
			{
				boost::mutex::scoped_lock lock(_mutex);

				while(_pInstance->getMsgQueue()->empty())
					_pInstance->_condition.wait(lock);

				if(!_pInstance->getMsgQueue()->empty())
				{
					for(unsigned int n=0;n<_pInstance->getMsgQueue()->size();n++)
					{
						_pInstance->request_process(&_pInstance->getMsgQueue()->front());
						_pInstance->getMsgQueue()->pop();
					}
				}
			}
			catch(thread_interrupted&)
			{
				break;
			}
		}
	}
}

} /* namespace plun */

