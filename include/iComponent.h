/*
 * iComponent.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef ICOMPONENT_H_
#define ICOMPONENT_H_

#include "plunTypedef.h"
#include "plunLog.h"
#include <string>
#include <fstream>

using namespace std;
using namespace boost;

namespace plun
{

class iComponent {
public:
	iComponent(const char* component_name)
	:_pInstance(nullptr),_pProfileFile(nullptr), _pProperty(nullptr), _pInBox(nullptr), _pComponent_info(nullptr), _comp_name(component_name) {
	}
	virtual ~iComponent() {

		if(_pInBox!=nullptr)
			delete _pInBox;

		if(_pProfileFile!=nullptr)
			delete _pProfileFile;

		if(_pComponent_info!=nullptr)
			delete _pComponent_info;

		if(_pProperty!=nullptr)
			delete _pProperty;
	}

	/*
	 * Component Run
	 * Run own main-thread
	 */
	virtual void run() = 0;

	/*
	 * Component Stop
	 * Stop own-main-thread (child-thread is stopped also.)
	 */
	virtual void stop() = 0;

	virtual void request_process(RequestMsg* msg) = 0;

	iComponent* getInstance() { return _pInstance; }
	ComponentInfo* getInfo()	{	return _pComponent_info;	}
	JValue* getProperty() { return _pProperty; }
	RequestMsgQueue* getMsgQueue() { return _pInBox; }


	const char* getName() { return _comp_name.c_str(); }

	void init()
	{
		LOG_INFO << _comp_name << "(Last Build : " << __DATE__ << " " << __TIME__ << ")";

		if(_pInBox==nullptr)
			_pInBox = new RequestMsgQueue;
	}

	bool loadProfile(const char* component_name)
	{
		_profilePath = "./"+string(component_name)+".profile";

		if(_pProfileFile==nullptr)
		{
			ifstream file(_profilePath.c_str());
			if(!file.good()) return false;

			JReader reader;
			stringstream file_buffer;
			file_buffer << file.rdbuf();

			_pProfileFile = new JValue;
			if(!reader.parse(file_buffer.str(), *_pProfileFile))
			{
				delete _pProfileFile;
				_pProfileFile = nullptr;
				file.close();

				return false;
			}

			if(_pComponent_info!=nullptr)
				delete _pComponent_info;
			_pComponent_info = new ComponentInfo(_pProfileFile);

			if(_pProperty!=nullptr)
				delete _pProperty;
			else
			{
				_pProperty = new JValue;
				*_pProperty = (*_pProfileFile)["@property"];
			}

		}

		return true;
	}

	void request(RequestMsg* msg)
	{
		boost::mutex::scoped_lock lock(_mutex);
		if(_pInBox!=nullptr)
		{
			_pInBox->push(*msg);
			_condition.notify_one();
		}
	}
	void request(const char* msg)
	{
		boost::mutex::scoped_lock lock(_mutex);
		if(_pInBox!=nullptr)
		{
			_pInBox->push(RequestMsg(msg));
			_condition.notify_one();
		}
	}
	void request(const char* msg, sockaddr_in* src)
	{
		boost::mutex::scoped_lock lock(_mutex);
		if(_pInBox!=nullptr)
		{
			_pInBox->push(RequestMsg(msg, src));
			_condition.notify_one();
		}
	}

	void post(const char* msg)
	{

	}

protected:
	iComponent* _pInstance;
	JValue* _pProfileFile;
	JValue* _pProperty;
	RequestMsgQueue* _pInBox;
	ComponentInfo* _pComponent_info;

	string _profilePath;
	string _componentPath;

public:
	boost::condition_variable _condition;

private:
	boost::mutex _mutex;
	string _comp_name;


};

typedef iComponent*(*createComponent)(void);
typedef void(*destroyComponent)(void);

}

#endif /* ICOMPONENT_H_ */
