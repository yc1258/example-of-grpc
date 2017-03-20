#include "client.h"

void client::start() {
	thread_ = std::thread(&client::AsyncCompleteRpc, this);
}
void client::AsyncCompleteRpc() {
	void* got_tag;
	bool ok = false;
	while (cq_.Next(&got_tag, &ok)) {
		AsyncClientCallBase* call = static_cast<AsyncClientCallBase*>(got_tag);
		GPR_ASSERT(ok);
		call->callCallback();
		delete call;
	}
}

int client::func1(const char *param1, int param2, uint16_t param3) {
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<   start grpc : func1   <<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	func1_Request request;
	request.set_param1(string(param1));
	request.set_param2(param2);
	request.set_param3((uint32_t)param3);

	auto call = new AsyncClientCall<func1_Reply>();
	call->callback = [](func1_Reply &reply_, Status &status_){
		if (status_.ok()) {
			auto id = boost::shared_ptr<Structure>(new Structure);
			id->param1 = reply_.id().param1();
			id->param2 = reply_.id().param2();
			/*
			do something
			*/
			return 0;
		}
		else {
			std::cout << "RPC func1 failed" << std::endl;
			return -1;
		}
	};
	call->response_reader = stub_->Asyncfunc1(&call->context, request, &cq_);
	call->response_reader->Finish(&call->reply, &call->status, (void*)call);
	return 0;
}

int client::func2(const string &param1) {
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<   start grpc : func2   <<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	func2_Request request;
	request.set_param1(param1);

	auto call = new AsyncClientCall<func2_Reply>();
	call->callback = [](func2_Reply &reply_, Status &status_) {
		if (status_.ok()) {
			auto id = boost::shared_ptr<Structure>(new Structure);
			id->param1 = reply_.id().param1();
			id->param2 = reply_.id().param2();
			/*
			do something
			*/
			return 0;
		}
		else {
			std::cout << "RPC func2 failed" << std::endl;
			return -1;
		}
	};
	call->response_reader = stub_->Asyncfunc2(&call->context, request, &cq_);
	call->response_reader->Finish(&call->reply, &call->status, (void*)call);
	return 0;
}

std::vector<std::pair<string, bool> > client::func3() {
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<   start grpc : func3   <<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	std::vector < std::pair<string, bool> > clist;

	func3_Request request;
	request.set_message(1);
	func3_Reply reply;
	ClientContext context;
	Status status = stub_->func3(&context, request, &reply);

	if (status.ok()) {
		for (int i = 0; i < reply.result_size(); i++)
		{
			clist.push_back(std::make_pair(reply.result(i).param1(), reply.result(i).param2()));
		}
	}
	else {
		std::cout << "RPC func3 failed" << std::endl;
	}
	return clist;
}