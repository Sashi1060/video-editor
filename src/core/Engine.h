#pragma once

#include "Timeline.h"
#include "VideoDecoder.h"
#include <memory>
#include <map>

namespace ve::core {

class Engine {
public:
    Engine();
    ~Engine();

    void initialize();
    
    // Timeline operations
    void addClip(const std::string& filePath);
    Timeline& getTimeline() { return timeline_; }
    
    // Rendering
    std::shared_ptr<RawFrame> render(int64_t timestampUs);

private:
    Timeline timeline_;
    std::map<std::string, std::unique_ptr<VideoDecoder>> decoders_;
    
    VideoDecoder* getDecoder(const std::string& filePath);
};

} // namespace ve::core
