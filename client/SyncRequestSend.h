// Author: Zhenghong Yu
// Date: 07/06/2023
#pragma once

#include <thread>
#include <utility>

#include "grpcpp/server_builder.h"
#include "grpcpp/create_channel.h"
#include "services/request.pb.h"
#include "services/request.grpc.pb.h"

#include "common/Util.h"

class SyncRequestStub {

public:
    SyncRequestStub() {
        std::string target_address = util::GetAddress(target_ip_, target_port_);
        auto target_channel = grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials());
        stub_ = client::RequestService::NewStub(target_channel);
    }

    std::uint64_t Submit(client::Request* request) {
        grpc::ClientContext context;
        google::protobuf::Empty response;
        grpc::Status status = stub_->Submit(&context, *request, &response);
        CHECK(status.ok());
        return (util::Now() - request->temp_timestamp());
    }

private:
    std::string target_ip_ = "0.0.0.0";
    uint32_t target_port_ = 50051;
    std::unique_ptr<client::RequestService::Stub> stub_;
};


