/*
 * plunIO.cpp
 *
 *  Created on: 14. 08. 19
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#include "plunIO.h"
#include "../../include/plunLog.h"

namespace plun {

COMPONENT_INSTANCE(plunIO)
COMPONENT_CREATE(plunIO)
COMPONENT_DESTROY

plunIO::plunIO()
:iComponent(COMPONENT(plunIO)) {

}

plunIO::~plunIO() {

}

/*
* call once when it be started
*/
void plunIO::run()
{

}

/*
* call once when it be stopped
*/
void plunIO::stop()
{

}

/*
* if Request Queue is not empty, request_process will be called.
*/
void plunIO::request_process(sRequestMsg* msg)
{

}

} /* namespace plun */

