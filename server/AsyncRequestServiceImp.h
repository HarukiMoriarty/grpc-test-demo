// Author: Zhenghong Yu
// Date: 07/09/2023

#pragma once
#include <grpcpp/impl/codegen/completion_queue.h>
#include <thread>
#include <grpcpp/server_builder.h>
#include "services/request.grpc.pb.h"
#include "config/config.h"
#include "common/Util.h"

class AsyncRequestService {
public:
    ~AsyncRequestService() {
        server_->Shutdown();
        cq_->Shutdown();
    }

    void Run() {
        grpc::ServerBuilder builder;
        builder.AddListeningPort(util::GetAddress("0.0.0.0", 50051), grpc::InsecureServerCredentials());
        builder.RegisterService(&service_);
        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        LOG(INFO) << "Server sync service listening at " << util::GetAddress("0.0.0.0", 50051);
        HandleRpcs();
    }

private:
    class CallData {
    public:
        CallData(client::RequestService::AsyncService* service, grpc::ServerCompletionQueue* cq)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE) {
            Proceed();
        }

        void Proceed() {
            if (status_ == CREATE) {
                status_ = PROCESS;
                service_->RequestSubmit(&ctx_, &request_, &responder_, cq_, cq_, this);
            }
            else if (status_ == PROCESS) {
                new CallData(service_, cq_);
                status_ = FINISH;
                responder_.Finish(reply_, grpc::Status::OK, this);
            }
            else {
                GPR_ASSERT(status_ == FINISH);
                delete this;
            }
        }

    private:
        client::RequestService::AsyncService* service_;
        grpc::ServerCompletionQueue* cq_;
        grpc::ServerContext ctx_;
        client::Request request_;
        google::protobuf::Empty reply_;
        grpc::ServerAsyncResponseWriter<google::protobuf::Empty> responder_;
        enum CallStatus { CREATE, PROCESS, FINISH };
        CallStatus status_;
    };

    void HandleRpcs() {
        new CallData(&service_, cq_.get());
        void* tag;
        bool ok;
        while (true) {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<CallData*>(tag)->Proceed();
        }
        LOG(INFO) << "Stop handling rpcs";
    }

private:
    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    client::RequestService::AsyncService service_;
    std::unique_ptr<grpc::Server> server_;
};