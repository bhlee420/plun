/*
 * plunIO.h
 *
 *  Created on: 14. 08. 19
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 *		 Desc. : plun I/O Handle
 */

#ifndef PLUNIO_H_
#define PLUNIO_H_

#include "../../include/plun.h"
#include "../../include/plunTypedef.h"


namespace plun {

class plunIO : public iComponent {
public:
	plunIO();
	virtual ~plunIO();

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

