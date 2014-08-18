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
	if(g_broker!=nullptr)
		g_broker->destroy();

	if(g_container!=nullptr)
		g_container->destroy();
}

void signal_handler(int h)
{
	destroyInstance();
	exit(0);
}

int main(int argc, const char* argv[])
{
	LOG_INFO << "PLUN(Last Build : " << __DATE__ << " " << __TIME__ << ")";

	signal(SIGINT, signal_handler);

	char component_list[1024] = {0,};
	memset(component_list, 0x00, sizeof(component_list));

	struct poptOption optionTable[] = {
			{"with components installation", 'i', POPT_ARG_STRING, component_list, 'i', "install components", NULL},
			{"test", 't', POPT_ARG_NONE, component_list, 't', "install components", NULL},
			POPT_AUTOHELP
			POPT_TABLEEND
	};
	poptContext optionCon = poptGetContext(NULL, argc, argv, optionTable, 0);
	poptSetOtherOptionHelp(optionCon, "[OPTION] *");

	createInstnace();

	int opt;
	while((opt = poptGetNextOpt(optionCon))>=0)
	{
		switch(opt)
		{
		case 'i':

			/* Install components */
			for(int n=2; n<argc; n++)
				g_container->install(argv[n]);

			/* run all components */
			g_container->run_all();

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

	destroyInstance();
	poptFreeContext(optionCon);

	return 0;
}
