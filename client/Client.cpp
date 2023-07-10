// Author: Zhenghong Yu
// Date: 07/09/2023

#include <iostream>
#include "SyncRequestSend.h"
#include "WorkManager.h"
#include "config/config.h"
#include "services/request.pb.h"
#include "services/request.grpc.pb.h"
#include "common/Util.h"
#include <thread>
#include <atomic>


void run_workload(void) {
    WorkManager work_manager;
    SyncRequestStub client;
    std::uint64_t cnt = 0;
    std::uint64_t latency_sum{ 0 };
    std::uint64_t start_time{ 0 };
    std::uint64_t end_time{ 0 };
    std::uint64_t request_number{ 0 };



    while (cnt < config::statistic_number) {
        client::Request msg;
        if (cnt == config::warm_up_number) {
            start_time = util::Now();
        }
        work_manager.SetRequest(&msg);
        std::uint64_t latency = client.Submit(&msg);
        if (cnt >= config::warm_up_number) {
            latency_sum += latency;
            request_number += 1;
        }
        cnt++;
    }
    end_time = util::Now();
    LOG(INFO) << "latency(ms)=" << 1.0 * latency_sum / (1000000 * request_number)
        << "\t throughput(requests/second)=" << 1.0 * request_number * 1000000000 * config::client_num / (latency_sum);
}


int main(int argc, char* argv[]) {
    std::thread client_threads[config::client_num];
    for (int i = 0; i < config::client_num; i++) {
        client_threads[i] = std::thread(&run_workload);
    }

    for (auto& client_thread : client_threads) {
        client_thread.join();
    }


}