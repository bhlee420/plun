/*
 * cameraOpenCV.cpp
 *
 *  Created on: 2014. 8. 20.
 *      Author: hwang-linux
 */

#include "cameraOpenCV.h"

namespace plun {

cameraOpenCV::cameraOpenCV(int cameraID, unsigned int width, unsigned height)
:iCamera(-1, width, height), _captured_image(nullptr) {

	_capture = cvCaptureFromCAM(cameraID);

	if(_capture)
	{
		cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_WIDTH, width);
		cvSetCaptureProperty(_capture, CV_CAP_PROP_FRAME_HEIGHT, height);
	}

}

cameraOpenCV::~cameraOpenCV() {
	if(_capture)
		cvReleaseCapture(&_capture);
}


bool cameraOpenCV::capture()
{
	if(_capture && cvGrabFrame(_capture))
		return true;

	return false;
}


bool cameraOpenCV::retrieve()
{
	if(_capture)
	{
		_captured_image = cvRetrieveFrame(_capture);
		return true;
	}
	return false;
}


const char* cameraOpenCV::getImage()
{
	if(_capture)
	{
		_captured_image = cvRetrieveFrame(_capture);
		return _capture
	}

	return nullptr;
}

} /* namespace plun */
