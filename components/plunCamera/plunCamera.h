/*
 * plunCamera.h
 *
 *  Created on: 14. 08. 20
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : plun Camera
 */

#ifndef PLUNCAMERA_H_
#define PLUNCAMERA_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"
#include "../../include/device/iCamera.h"


namespace plun {

class iCamera;
class plunCamera : public iComponent {
public:
	plunCamera();
	virtual ~plunCamera();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* msg);

	/*
	 * camera interface
	 */


private:
	iCamera* _camera;
	Task _cameraTask;

private:
	void loop();

};

COMPONENT_EXPORT

} /* namespace plun */

#endif

