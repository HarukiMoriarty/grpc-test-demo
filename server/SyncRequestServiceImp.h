// Author: Zhenghong Yu
// Date: 07/10/2023

#include <grpcpp/impl/codegen/completion_queue.h>
#include <thread>
#include <grpcpp/server_builder.h>
#include "services/request.grpc.pb.h"
#include "config/config.h"
#include "common/Util.h"

class SyncRequestServiceImpl final : public client::RequestService::Service {
    grpc::Status Submit(grpc::ServerContext* context, const client::Request* request, google::protobuf::Empty* reply) override {
        return grpc::Status::OK;
    }
};

void RunServer(void) {
    SyncRequestServiceImpl service;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(util::GetAddress("0.0.0.0", 50051), grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    LOG(INFO) << "Server sync service listening at " << util::GetAddress("0.0.0.0", 50051);
    server->Wait();
}