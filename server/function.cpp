
#include "function.h"

boost::shared_ptr<function> function::func_instance;

std::map<sss_oper_t *, boost::any> function::callbakMap_;

boost::shared_ptr<function> function::get_instance()
{
	if(!func_instance)
	{
		func_instance =boost::shared_ptr<function>(new function);
	}
	
	return func_instance;
}

int function::func1(const char *param1, int param12, uint16_t param3, const SipInviteCallback &cb)
{	
	int tag;
	callbakMap_[tag] = cb;
	/*
	do something
	*/
    return 0;
}

struct_ptr function::func2(string param1)
{	
	struct_ptr id = boost::shared_ptr<Structure>(new Structure);
	/*
	do something
	*/
    return id;
}

std::vector<std::pair<string, bool> > function::func3()
{
	std::vector<std::pair<string, bool> > clist;
	std::map<string, bool>::iterator it;
	for (it = getlist.begin(); it != getlist.end(); it++)
	{
		clist.push_back(std::make_pair(it->first, it->second));
	}
	return clist;
}

void function::func1callback(int tag,int type)
{
	/*
	do something
	*/
	if (callbakMap_.find(tag) != callbakMap_.end())
	{
		SipInviteCallback callback = boost::any_cast<SipInviteCallback>(callbakMap_[tag]);
		callbakMap_.erase(tag);
		callback(id, type);
	}
}
