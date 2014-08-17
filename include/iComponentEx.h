/*
 * iComponentEx.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef ICOMPONENTEX_H_
#define ICOMPONENTEX_H_

#include "iComponent.h"
#include "plunLog.h"
#include "plunTypedef.h"

using namespace std;
using namespace boost;

namespace plun {

class iComponentEx : public iComponent {
public:
	iComponentEx(const char* component_name);
	virtual ~iComponentEx();

	virtual void run();
	virtual void stop();
	virtual void request_process(RequestMsg* uri) {};

private:
	void comp_inner_loop();

private:
	void* _handle;
	boost::shared_ptr<boost::thread> _thread;
	boost::mutex _mutex;
};

} /* namespace plun */

#endif /* ICOMPONENTEX_H_ */



