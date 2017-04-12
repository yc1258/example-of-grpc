#ifndef SERVER_FUNC_H
#define SERVER_FUNC_H
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
typedef std::function<void(const struct_ptr, int pt)> InviteCallback;

class server_func
{	
	public:
		~server_func() {};
		static boost::shared_ptr<server_func> get_instance();
		//dosomething and get the result till the callback arrived
		int func1(const char *param1, int param2, uint16_t param3, const InviteCallback &cb);
		//dosomething and get the result right away
		struct_ptr func2(string id);
		//a request to get some information
		std::vector<std::pair<string, bool> > func3();
		void func1callback();
		std::map<string, bool> list_pair;
	private:
		server_func() {};
		std::map<string, bool> getlist;
		static std::map<int, boost::any> callbakMap_;
};	
#endif
