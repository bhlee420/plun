/*
 * appFaceDetection.cpp
 *
 *  Created on: 14. 08. 20
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "appFaceDetection.h"
#include "../../include/plunLog.h"

namespace plun {

COMPONENT_INSTANCE(appFaceDetection)
COMPONENT_CREATE(appFaceDetection)
COMPONENT_DESTROY

appFaceDetection::appFaceDetection()
:iComponent(COMPONENT(appFaceDetection)) {

}

appFaceDetection::~appFaceDetection() {

}

/*
* call once when it be started
*/
void appFaceDetection::run()
{

}

/*
* call once when it be stopped
*/
void appFaceDetection::stop()
{

}

/*
* if Request Queue is not empty, request_process will be called.
*/
void appFaceDetection::request_process(sRequestMsg* msg)
{
	switch(msg->method)
	{
	case Method::GET:
	{
		if(msg->msg.dest_comp=="appFaceDetection" && msg->msg.function=="getFaceArea")
		{
			iComponent* _fdComponent = componentBroker::getBroker()->getComponent("plunFaceDetect");
			if(_fdComponent!=nullptr)
				_fdComponent->request("@plunFaceDetect/detect");
		}
	}
	break;
	case Method::POST:
	case Method::DELETE:
	case Method::PUT:
		break;
	}
}

} /* namespace plun */

