/*
 * iFaceDetection.h
 *
 *  Created on: 2014. 8. 20.
 *      Author: hwang-linux
 */

#ifndef IFACEDETECTION_H_
#define IFACEDETECTION_H_

#include <vector>
#include "../plunTypedef.h"

using namespace std;

namespace plun
{

class iFaceDetection
{
public:
	iFaceDetection() { }
	virtual ~iFaceDetection() { }

	/*
	 * initialization
	 */
	virtual void init() = 0;

	/*
	 * detected area
	 */
	virtual vector<Rectangle> detect(unsigned char* image, unsigned int width, unsigned height) = 0;
};

}




#endif /* IFACEDETECTION_H_ */
