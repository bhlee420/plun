/*
 * sampletag.h
 *
 *  Created on: 14. 07. 23
 *      Author: Nsynapse Inc.
 *		 Desc. : Sample Component by CG Tool
 */

#ifndef sampletag_H_
#define sampletag_H_

#include "../../plun.h"
#include "../../plunTypedef.h"


namespace plun {

class sampletag : public iComponent {
public:
	sampletag();
	virtual ~sampletag();

	/*
	 * component interface
	 */
	virtual void run();
	virtual void stop();
	virtual void request_process(URI* uri);
};

COMPONENT_EXPORT

} /* namespace plun */

#endif

