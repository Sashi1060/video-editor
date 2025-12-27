#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace ve::core {

struct MediaClip {
    std::string filePath;
    int64_t duration = 0; // microseconds
    int64_t inPoint = 0;  // microseconds
    int64_t outPoint = 0; // microseconds
    int width = 0;
    int height = 0;
    double fps = 0.0;
    
    // Timeline position
    int64_t timelineStart = 0; // microseconds

    int64_t length() const {
        return outPoint - inPoint;
    }
};

struct RawFrame {
    std::vector<uint8_t> data;
    int width;
    int height;
    int linesize;
    int64_t pts; // microseconds
};

} // namespace ve::core
