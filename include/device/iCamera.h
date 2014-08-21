/*
 * iCamera.h
 *
 *  Created on: 2014. 8. 20.
 *      Author: hwang-linux
 */

#ifndef ICAMERA_H_
#define ICAMERA_H_

namespace plun
{

#define RESOLUTION_160_120		160, 120
#define RESOLUTION_320_240		320, 240
#define RESOLUTION_640_480		640, 480
#define RESOLUTION_800_600		800, 600
#define RESOLUTION_1024_768		1024, 768
#define RESOLUTION_1280_1024	1280, 1024
#define RESOLUTION_1600_1200	1600, 1200

class iCamera
{
public:
	iCamera(int cameraID = 0, unsigned int width = 640, unsigned height = 480)
	:_cameraID(0), _width(width), _height(height) { }
	virtual ~iCamera() {}

	/*
	 * interface function for still image capture
	 */
	virtual bool capture() = 0;

	/*
	 * getting image data from camera memory buffer
	 */
	virtual bool retrieve() = 0;

	/*
	 * return image pointer
	 */
	virtual const unsigned char* getImage() = 0;

protected:
	int _cameraID;
	unsigned int _width;
	unsigned int _height;
};

}



#endif /* ICAMERA_H_ */
