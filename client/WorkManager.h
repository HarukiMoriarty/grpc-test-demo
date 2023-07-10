// Author: Zhenghong Yu
// Date: 07/09/2023

#pragma once

#include "common/Util.h"
#include "services/request.pb.h"
#include "services/request.grpc.pb.h"

class WorkManager {

public:
    WorkManager() {};

    ~WorkManager() {};

    void SetRequest(client::Request* request) {
        request->set_islast(false);
        request->add_keys(0);
        request->add_iswrite(false);
        request->add_values("123456789");
        request->set_localid(0);
        request->set_globalid(0);
        request->set_priorityid(0);
        request->set_waitcounter(0);
        request->set_client_address("123456789");
        request->set_rerun_times(0);
        request->set_temp_timestamp(util::Now());

        return;
    }

};