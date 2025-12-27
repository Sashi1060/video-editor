#pragma once

#include "FFmpegUtils.h"
#include "MediaClip.h"
#include <string>
#include <vector>
#include <memory>

namespace ve::core {

class VideoDecoder {
public:
    VideoDecoder();
    ~VideoDecoder();

    void open(const std::string& filePath);
    bool seek(int64_t timestampUs);
    std::shared_ptr<RawFrame> decodeNextFrame();
    
    // Helper to get specific frame (seek + decode)
    std::shared_ptr<RawFrame> getFrameAt(int64_t timestampUs);

    const MediaClip& getClipInfo() const { return clipInfo_; }

private:
    void initSwsContext(int width, int height, AVPixelFormat format);

    AVFormatContextPtr formatCtx_;
    AVCodecContextPtr codecCtx_;
    int videoStreamIndex_ = -1;
    
    SwsContextPtr swsCtx_;
    AVFramePtr frame_;
    AVFramePtr rgbFrame_;
    AVPacketPtr packet_;
    
    MediaClip clipInfo_;
    std::vector<uint8_t> rgbBuffer_;
};

} // namespace ve::core
