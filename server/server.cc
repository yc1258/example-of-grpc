#include <memory>
#include <iostream>
#include <string>
#include <thread>

#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "server_func.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using namespace proto;

class ServerImpl final {
public:
	~ServerImpl() {
		server_->Shutdown();
		cq_->Shutdown();
	}

	void Run() {
		std::string server_address("0.0.0.0:50051");

		ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&service_);

		cq_ = builder.AddCompletionQueue();
		server_ = builder.BuildAndStart();
		std::cout << "Server listening on " << server_address << std::endl;
		HandleRpcs();
	}

private:
	class CallData {
	public:
		enum RPCType {
			T_func1 = 0,
			T_func2,
			T_func3,
		};

		CallData(Proto::AsyncService* service, ServerCompletionQueue* cq, RPCType type)
			: service_(service), cq_(cq), type_(type), func1_responder_(&ctx_), func2_responder_(&ctx_), func3_responder_(&ctx_), status_(CREATE) {
			Proceed();
		}
		void Proceed() {
			if (status_ == CREATE) {
				status_ = PROCESS;
				switch (type_)
				{
				case T_func1:
					service_->Requestfunc1(&ctx_, &func1_request_, &func1_responder_, cq_, cq_,this);
					break;
				case T_func2:
					service_->Requestfunc2(&ctx_, &func2_request_, &func2_responder_, cq_, cq_, this);
					break;
				case T_func3:
					service_->Requestfunc3(&ctx_, &func3_request_, &func3_responder_, cq_, cq_, this);
					break;
				default:
					break;
				}
			}
			else if (status_ == PROCESS) {
				new CallData(service_, cq_, this->type_);
				switch (type_)
				{
				case T_func1: {
					function::get_instance()->func1(func1_request_.param1().c_str(), func1_request_.param2(), (uint16_t)func1_request_.param3(),
						[=](const struct_ptr id, const int type) {
						Structure *id4cli = func1_reply_.mutable_id();
						id4cli->set_param1(id->param1);
						id4cli->set_param2(id->param2);

						func1_reply_.set_type(type);
						status_ = FINISH;
						func1_responder_.Finish(func1_reply_, Status::OK, this);
					});
					break;
				}
				case T_func2: {
					struct_ptr id = function::get_instance()->func2(func2_request_.param1());
					Structure *id4cli = func2_reply_.mutable_id();
					id4cli->set_param1(id->param1);
					id4cli->set_param2(id->param2);

					status_ = FINISH;
					func2_responder_.Finish(func2_reply_, Status::OK, this);
					break;
				}
				case T_func3: {
					std::vector<std::pair<string, bool> > clist = function::get_instance()->func3();
					for (auto iter = clist.begin(); iter != clist.end(); iter++)
					{
						list_pair *cp = func3_reply_.add_result();
						cp->set_param1(iter->first);
						cp->set_param2(iter->second);
					}
					status_ = FINISH;
					func3_responder_.Finish(func3_reply_, Status::OK, this);
					break;
				}
				default:
					break;
				}
			}
			else {
				GPR_ASSERT(status_ == FINISH);
				delete this;
			}
		}

	private:
		SPS::AsyncService* service_;
		ServerCompletionQueue* cq_;
		ServerContext ctx_;
		RPCType type_;

		func1_Request func1_request_;
		func1_Reply func1_reply_;
		func2_Request func2_request_;
		func2_Reply func2_reply_;
		func3_Request func3_request_;
		func3_Reply func3_reply_;

		ServerAsyncResponseWriter<func1_Reply> func1_responder_;
		ServerAsyncResponseWriter<func2_Reply> func2_responder_;
		ServerAsyncResponseWriter<func3_Reply> func3_responder_;

		enum CallStatus { CREATE, PROCESS, FINISH };
		CallStatus status_;
	};

	void HandleRpcs() {
		new CallData(&service_, cq_.get(), ServerImpl::CallData::T_func1);
		new CallData(&service_, cq_.get(), ServerImpl::CallData::T_func2);
		new CallData(&service_, cq_.get(), ServerImpl::CallData::T_func3);

		void* tag; 
		bool ok;
		while (true) {
			GPR_ASSERT(cq_->Next(&tag, &ok));
			GPR_ASSERT(ok);
			static_cast<CallData*>(tag)->Proceed();
		}
	}
	std::unique_ptr<ServerCompletionQueue> cq_;
	SPS::AsyncService service_;
	std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
	ServerImpl server;
	server.Run();

	return 0;
}
