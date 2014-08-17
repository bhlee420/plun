/*
 * plunner.cpp
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */


#include <popt.h>
#include <csignal>
#include <cassert>

#include "../include/plun.h"
#include "../include/plunLog.h"
#include "../include/plunTypedef.h"
#include "../include/componentBroker.h"
#include "../include/componentContainer.h"

using namespace plun;
using namespace std;

componentContainer* g_container = nullptr;
componentBroker* g_broker = nullptr;


void createInstnace()
{
	g_container = componentContainer::getContainer();
	g_broker = componentBroker::getBroker();
}

void destroyInstance()
{
	g_broker->destroy();
	g_container->destroy();
}

void signal_handler(int h)
{
	destroyInstance();
	exit(0);
}

void default_setup()
{
	g_container->install("plunHost");	//must be installed to connect with this host & services.
	g_container->run_all();
}

int main(int argc, const char* argv[])
{
	LOG_INFO << "PLUN(Last Build : " << __DATE__ << " " << __TIME__ << ")";

	signal(SIGINT, signal_handler);

	struct poptOption optionTable[] = {
			{"run", 'r', POPT_ARG_NONE, NULL, 'r', "Run Plunner", NULL},
			POPT_AUTOHELP
			POPT_TABLEEND
	};
	poptContext optionCon = poptGetContext(NULL, argc, argv, optionTable, 0);
	poptSetOtherOptionHelp(optionCon, "[OPTION] *");

	if(argc<2)
	{
		poptPrintUsage(optionCon, stderr, 0);
		return 1;
	}

	int opt;
	while((opt = poptGetNextOpt(optionCon))>=0)
	{
		switch(opt)
		{
		case 'r':

			createInstnace();
			default_setup();

			while(1)
			{
				try
				{
					this_thread::sleep(posix_time::milliseconds(500));
				}
				catch(thread_interrupted&)
				{
					break;
				}
			}
			break;

		}
	}

	poptFreeContext(optionCon);
	destroyInstance();

	return 0;
}
