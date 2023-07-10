// Author: Zhenghong Yu
// Date: 07/06/2023
#pragma once
#include "glog/logging.h"
#include "chrono"

namespace util {

    static std::uint64_t Now() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    static std::string GetAddress(std::string ip, uint32_t port) {
        return ip + ":" + std::to_string(port);
    }
}// namespace util

