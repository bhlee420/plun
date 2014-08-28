/*
 * plun.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef PLUN_H_
#define PLUN_H_

#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif

#include "iComponent.h"
#include "componentBroker.h"

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/bind.hpp>

using namespace boost;
using namespace std;

namespace plun
{

/*
 * MACRO
 */
#define COMPONENT_EXPORT		extern "C" iComponent* create(); extern "C" void destroy(void);
#define COMPONENT_INSTANCE(classname)	static classname* _instance = nullptr;
#define COMPONENT_CREATE(classname) iComponent* create(){ if(_instance==nullptr) _instance = new classname(); return _instance;}
#define COMPONENT_DESTROY void destroy(){ if(_instance!=nullptr){delete _instance; _instance=nullptr;} }
#define COMPONENT(classname) #classname

/*
 * Global definitions
 */
#define __PLUN_VERSION__		"0.0.1"

}


#endif /* PLUN_H_ */
