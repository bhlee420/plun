/*
 * appFaceDetection.h
 *
 *  Created on: 14. 08. 20
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : Face Detection App.
 */

#ifndef APPFACEDETECTION_H_
#define APPFACEDETECTION_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"


namespace plun {

class appFaceDetection : public iComponent {
public:
	appFaceDetection();
	virtual ~appFaceDetection();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* msg);


};

COMPONENT_EXPORT

} /* namespace plun */

#endif

