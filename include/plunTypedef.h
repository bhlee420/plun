/*
 * plunTypedef.h
 *
 *  Created on: 2014. 7. 3.
 *      Author: Byunghun Hwang<bhhwang@nsynapse.com>
 */

#ifndef PLUNTYPEDEF_H_
#define PLUNTYPEDEF_H_

#include <string>
#include <vector>
#include <queue>
#include <array>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <json/json.h>
#include <netinet/in.h>

using namespace std;
using namespace boost;
using namespace Json;


namespace plun
{
	typedef Json::Value	JValue;
	typedef Json::Reader	JReader;

	typedef boost::shared_ptr<boost::thread> Task;
	#define TaskRegister(funcptr) boost::thread(boost::bind(funcptr, this))

	typedef enum { GET=11, POST, PUT, DELETE } Method;

	typedef struct sMsg
	{
		string raw;
		string dest_comp;
		string function;
		unordered_map<string, string> args;
		sMsg() {}
		sMsg(const char* msg){ set(msg); }
		sMsg& operator=(const char* msg){ set(msg); return *this;}
	private:
		void set(const char* msg) {
			raw = string(msg);
			algorithm::to_lower(raw);
			regex msg_pattern("^(?:@)?([^/]+)/([^\?]+).?(.*)$");
			regex arg_pattern("([a-zA-Z0-9_-]+=[a-zA-Z0-9_-]+)(?:$|[ &]+)");
			smatch msg_match, args_match;

			if(regex_search(raw, msg_match, msg_pattern))
			{
				dest_comp = string(msg_match[1].first, msg_match[1].second);
				function = string(msg_match[2].first, msg_match[2].second);

				string args_value(msg_match[3].first, msg_match[3].second);
				if(args_value.size()>0)
				{
					string::const_iterator begin = args_value.begin();
					string::const_iterator end = args_value.end();
					vector<string> arglist;
					while(regex_search(begin, end, args_match, arg_pattern))
					{
						arglist.push_back(string(args_match[1].first, args_match[1].second));
						begin = args_match[0].second;
					}

					vector<string>::iterator itr = arglist.begin();
					for(;itr!=arglist.end();++itr)
					{
						std::vector<std::string> strs;
						split(strs, *itr, is_any_of("="));
						if(strs.size()>=2)
							args[strs[0]] = strs[1];
					}
				}
			}
		}
	} Msg;

	typedef struct sRequestMsg
	{
		Method method;
		Msg msg;
		struct sockaddr_in src_addr;
		struct sockaddr_in dest_addr;
		sRequestMsg(Method req_method, const char* req_msg, sockaddr_in src, sockaddr_in dest):method(req_method), msg(req_msg), src_addr(src), dest_addr(dest){}
		sRequestMsg(const char* req_msg):method(Method::GET), msg(req_msg) { }
		sRequestMsg(const char* req_msg, sockaddr_in* src):method(Method::GET), msg(req_msg),src_addr(*src) { }
		sRequestMsg& operator=(const sRequestMsg& ref){
			method = ref.method;
			msg = ref.msg;
			src_addr=ref.src_addr;
			dest_addr = ref.dest_addr;
			return *this;
		}
	} RequestMsg;
	typedef queue<RequestMsg> RequestMsgQueue;


	typedef struct INFO
	{
	public:
		unordered_map<string, string> info;
		vector<string> tags;
		vector<string> packages;

		INFO(JValue* jprofile) {
			const JValue cv = (*jprofile)["@info"];
			const char* prof_list[] = {"gid", "name", "description", "author","author_email", "url", "license", "version", "category"};

			for(int i=0;i<(int)(sizeof(prof_list)/sizeof(char*));i++)
				insert_profile(&cv, prof_list[i]);
			insert(&cv, "tags", tags);
			insert(&cv, "packages", packages);
		}
	private:
			void insert_profile(const JValue* cv, const char* key){
				info.insert(unordered_map<string, string>::value_type(key,cv->get(key,"").asString()));
			}
			void insert(const JValue* cv, const char* key, vector<string>& t){
				const JValue v = (*cv)[key];
				for(int i=0;i<(int)v.size();i++)
					t.push_back(v[i].asString());

			}
	} ComponentInfo;

	typedef struct _RECT
	{
		int left_top_x, left_top_y;
		int right_bottom_x, right_bottom_y;
	} Rectangle;
}



#endif /* TYPEDEF_H_ */
