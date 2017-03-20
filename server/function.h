#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/any.hpp>
#include <thread>
#include "Protocol.grpc.pb.h"

using std::string;
using std::cout;
using std::endl;
using proto::Structure;

struct Structure
{
	string param1;
	string param2;
};
typedef boost::shared_ptr<Structure> struct_ptr;
typedef std::function<void(const sip_connect_ptr, int pt)> SipInviteCallback;

class function
{	
	public:
		~function() {};
		static boost::shared_ptr<function> get_instance();
		int func1(const char *param1, int param2, uint16_t param3, const SipInviteCallback &cb);
		struct_ptr func2(string id);
		std::vector<std::pair<string, bool> > func3();
		void func1callback();
		std::map<string, bool> list_pair;
	private:
		function() {};
		std::map<string, bool> getlist;
		static std::map<sss_oper_t *, boost::any> callbakMap_;
};	
#endif
