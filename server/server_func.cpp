
#include "server_func.h"

boost::shared_ptr<server_func> server_func::func_instance;

std::map<int, boost::any> server_func::callbakMap_;

boost::shared_ptr<server_func> server_func::get_instance()
{
	if(!func_instance)
	{
		func_instance =boost::shared_ptr<server_func>(new server_func);
	}
	
	return func_instance;
}

int server_func::func1(const char *param1, int param2, uint16_t param3, const InviteCallback &cb)
{	
	int tag;
	callbakMap_[tag] = cb;
	/*
	do something
	*/
    return 0;
}

struct_ptr server_func::func2(string param1)
{	
	struct_ptr id = boost::shared_ptr<Structure>(new Structure);
	/*
	do something
	*/
    return id;
}

std::vector<std::pair<string, bool> > server_func::func3()
{
	std::vector<std::pair<string, bool> > clist;
	std::map<string, bool>::iterator it;
	for (it = getlist.begin(); it != getlist.end(); it++)
	{
		clist.push_back(std::make_pair(it->first, it->second));
	}
	return clist;
}

void server_func::func1callback(int tag,int type)
{
	/*
	do something
	*/
	if (callbakMap_.find(tag) != callbakMap_.end())
	{
		InviteCallback callback = boost::any_cast<InviteCallback>(callbakMap_[tag]);
		callbakMap_.erase(tag);
		callback(id, type);
	}
}
