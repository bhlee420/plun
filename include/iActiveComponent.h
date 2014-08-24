/*
 * iActiveComponent.h
 *
 *  Created on: 2014. 8. 24.
 *      Author: Byunghun Hwang <bhhwang@nsynapse.com>
 *
 *      Desc. : ActiveComponent can transfer data responding to a process request
 */

#ifndef IACTIVECOMPONENT_H_
#define IACTIVECOMPONENT_H_


namespace plun
{

class iActiveComponent : protected iComponent
{
	iActiveComponent() {}
	virtual ~iActiveComponent() {}
};

}



#endif /* IACTIVECOMPONENT_H_ */
