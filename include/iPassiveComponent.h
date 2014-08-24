/*
 * iPassiveComponent.h
 *
 *  Created on: 2014. 8. 24.
 *      Author: Byunghun Hwang <bhhwang@nsynapse.com>
 *
 *      Desc. : PassiveComponent cannot transfer data responding to a process request
 */

#ifndef IPASSIVECOMPONENT_H_
#define IPASSIVECOMPONENT_H_

namespace plun
{

class iPassiveComponent : protected iComponent
{
	iPassiveComponent() {}
	virtual ~iPassiveComponent() {}
};

}



#endif /* IPASSIVECOMPONENT_H_ */
