
#ifndef _CLIENT_H
#define _CLIENT_H
#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <thread>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "Protocol.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using namespace proto;

using std::string;

class AsyncClientCallBase {
public:
	virtual void callCallback() = 0;
	virtual ~AsyncClientCallBase() {}
};

template<class T>
class AsyncClientCall :public AsyncClientCallBase {
public:
	T reply;
	ClientContext context;
	Status status;
	std::unique_ptr<ClientAsyncResponseReader<T>> response_reader;
	std::function<int(T&, Status&)> callback;
	virtual void callCallback() {
		callback(reply, status);
	}
};

class client{
public:
	explicit client(std::shared_ptr<Channel> channel)
		: stub_(Proto::NewStub(channel)) {}
	~client() {
		thread_.join();
	};
	void start();
	void AsyncCompleteRpc();
	//asynchronous functions
	int func1(const char *param1, int param2, uint16_t param3);
	int func2(const string &param1);
	//synchronous function
	std::vector<std::pair<string, bool> > func3();	
private:
	std::unique_ptr<Proto::Stub> stub_;
	CompletionQueue cq_;
	std::thread thread_;
};

#endif