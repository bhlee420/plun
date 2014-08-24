/*
 * cameraOpenCV.h
 *
 *  Created on: 2014. 8. 20.
 *      Author: hwang-linux
 */

#ifndef CAMERAOPENCV_H_
#define CAMERAOPENCV_H_

#include "../../include/device/iCamera.h"
#include "opencv2/opencv.hpp"

namespace plun {

class cameraOpenCV : public iCamera {
public:
	cameraOpenCV(int cameraID = 0, unsigned int width = 640, unsigned height = 480);
	virtual ~cameraOpenCV();

public:
	/*
	 * interface function for still image capture
	 */
	bool capture();

	/*
	 * getting image data from camera memory buffer
	 */
	bool retrieve();

	/*
	 * return image pointer
	 */
	const char* getImage();

private:
	CvCapture* _capture;
	IplImage* _captured_image;

};

} /* namespace plun */

#endif /* CAMERAOPENCV_H_ */
