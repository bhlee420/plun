/*
 * facedetectOpenCV.h
 *
 *  Created on: 2014. 8. 20.
 *      Author: hwang-linux
 */

#ifndef FACEDETECTOPENCV_H_
#define FACEDETECTOPENCV_H_

#include <vector>
#include "opencv2/opencv.hpp"
#include "../../include/plunTypedef.h"
#include "../../include/algorithm/iFaceDetection.h"

using namespace std;

namespace plun {

class facedetectOpenCV : public iFaceDetection {
public:
	facedetectOpenCV();
	virtual ~facedetectOpenCV();

	/*
	 * initialization
	 */
	void init();

	/*
	 * detected area
	 */
	vector<Rectangle> detect(unsigned char* image, unsigned int width, unsigned height);
};

} /* namespace plun */

#endif /* FACEDETECTOPENCV_H_ */
