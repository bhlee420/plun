/*
 * plunLog.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef PLUNLOG_H_
#define PLUNLOG_H_

#ifndef BOOST_LOG_DYN_LINK
#define BOOST_LOG_DYN_LINK
#endif

#include <boost/log/trivial.hpp>

namespace plun
{

#if defined(__linux__) || defined(__linux) || defined(linux)
	#ifdef DEBUG
		#define LOG_INFO			BOOST_LOG_TRIVIAL(info)<<"@"<<__FILE__<<"("<<__LINE__<<") "<<" "
		#define LOG_ERROR			BOOST_LOG_TRIVIAL(error)<<"@"<<__FILE__<<"("<<__LINE__<<") "<<" "
		#define LOG_WARNING		BOOST_LOG_TRIVIAL(warning)<<"@"<<__FILE__<<"("<<__LINE__<<") "<<" "
		#define LOG_FATAL			BOOST_LOG_TRIVIAL(fatal)<<"@"<<__FILE__<<"("<<__LINE__<<") "<<" "
		#define LOG_DEBUG			BOOST_LOG_TRIVIAL(debug)<<"@"<<__FILE__<<"("<<__LINE__<<") "<<" "
	#else
		#define LOG_INFO			BOOST_LOG_TRIVIAL(info)<<"@"<<__PRETTY_FUNCTION__<<" "
		#define LOG_ERROR			BOOST_LOG_TRIVIAL(error)<<"@"<<__PRETTY_FUNCTION__<<" "
		#define LOG_WARNING		BOOST_LOG_TRIVIAL(warn)<<"@"<<__PRETTY_FUNCTION__<<" "
		#define LOG_CRITICAL		BOOST_LOG_TRIVIAL(critical)<<"@"<<__PRETTY_FUNCTION__<<" "
		#define LOG_DEBUG			BOOST_LOG_TRIVIAL(debug)<<"@"<<__PRETTY_FUNCTION__<<" "
	#endif
#endif

}


#endif /* PLUNLOG_H_ */
