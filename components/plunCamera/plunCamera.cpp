/*
 * plunCamera.cpp
 *
 *  Created on: 14. 08. 20
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunCamera.h"
#include "../../include/plunLog.h"
#include "cameraOpenCV.h"

namespace plun {

COMPONENT_INSTANCE(plunCamera)
COMPONENT_CREATE(plunCamera)
COMPONENT_DESTROY

plunCamera::plunCamera()
:iComponent(COMPONENT(plunCamera)),_camera(nullptr) {

}

plunCamera::~plunCamera() {

}

/*
* call once when it be started
*/
void plunCamera::run()
{
	if(_camera==nullptr)
	{
		_camera = new cameraOpenCV(0, 640, 480);
		_cameraTask = Task(new TaskRegister(&plunCamera::loop));
	}

}

/*
* call once when it be stopped
*/
void plunCamera::stop()
{
	if(_camera)
	{
		delete _camera;
		_camera = nullptr;
	}
}

/*
* if Request Queue is not empty, request_process will be called.
*/
void plunCamera::request_process(sRequestMsg* msg)
{
	switch(msg->method)
	{
	case Method::GET:
		break;
	case Method::POST:break;
	case Method::DELETE:break;
	case Method::PUT:	break;
	}
}

void plunCamera::loop()
{

}


} /* namespace plun */

