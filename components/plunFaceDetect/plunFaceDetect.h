/*
 * plunFaceDetect.h
 *
 *  Created on: 14. 08. 20
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : plun Face Detection algorithm(Adaboost) using openCV
 */

#ifndef PLUNFACEDETECT_H_
#define PLUNFACEDETECT_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"
#include "../../include/algorithm/iFaceDetection.h"


namespace plun {

class iFaceDetection;
class plunFaceDetect : public iComponent {
public:
	plunFaceDetect();
	virtual ~plunFaceDetect();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(sRequestMsg* msg);

	/*
	 * service function interface
	 */





private:
	iFaceDetection* _faceDetection;

};

COMPONENT_EXPORT

} /* namespace plun */

#endif

