#pragma once

#include <string>
#include <functional>

namespace ve::core {

class Exporter {
public:
    // Export a trimmed section of a video file
    // inPointUs, outPointUs: microseconds
    // progressCallback: returns percentage 0.0-1.0
    static void exportTrim(const std::string& inputFile, 
                           const std::string& outputFile, 
                           int64_t inPointUs, 
                           int64_t outPointUs,
                           std::function<void(float)> progressCallback = nullptr);
};

} // namespace ve::core
