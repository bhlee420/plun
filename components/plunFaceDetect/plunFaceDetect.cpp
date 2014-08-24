/*
 * plunFaceDetect.cpp
 *
 *  Created on: 14. 08. 20
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunFaceDetect.h"
#include "../../include/plunLog.h"
#include "facedetectOpenCV.h"

namespace plun {

COMPONENT_INSTANCE(plunFaceDetect)
COMPONENT_CREATE(plunFaceDetect)
COMPONENT_DESTROY

plunFaceDetect::plunFaceDetect()
:iComponent(COMPONENT(plunIO)), _faceDetection(nullptr) {

}

plunFaceDetect::~plunFaceDetect() {

}

/*
* call once when it be started
*/
void plunFaceDetect::run()
{
	if(_faceDetection==nullptr)
		_faceDetection = new facedetectOpenCV();
}

/*
* call once when it be stopped
*/
void plunFaceDetect::stop()
{
	if(_faceDetection!=nullptr)
	{
		delete _faceDetection;
		_faceDetection = nullptr;
	}
}

/*
* if Request Queue is not empty, request_process will be called.
*/
void plunFaceDetect::request_process(sRequestMsg* msg)
{
	switch(msg->method)
	{
	case Method::GET:
	{
		if(msg->msg.dest_comp=="plunFaceDetect" && msg->msg.function=="getFace")
		{
			//componentBroker::getBroker()->request()
		}
	}
		break;
	case Method::POST:break;
	case Method::DELETE:break;
	case Method::PUT:	break;
	}
}

} /* namespace plun */

