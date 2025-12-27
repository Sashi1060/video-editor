#pragma once

#include "FFmpegHeaders.h"
#include <memory>
#include <string>
#include <stdexcept>

namespace ve::core {

// Custom deleters
struct AVFormatContextDeleter {
    void operator()(AVFormatContext* ctx) const {
        if (ctx) avformat_close_input(&ctx);
    }
};

struct AVCodecContextDeleter {
    void operator()(AVCodecContext* ctx) const {
        if (ctx) avcodec_free_context(&ctx);
    }
};

struct AVFrameDeleter {
    void operator()(AVFrame* frame) const {
        if (frame) av_frame_free(&frame);
    }
};

struct AVPacketDeleter {
    void operator()(AVPacket* pkt) const {
        if (pkt) av_packet_free(&pkt);
    }
};

struct SwsContextDeleter {
    void operator()(SwsContext* ctx) const {
        if (ctx) sws_freeContext(ctx);
    }
};

// Unique pointers
using AVFormatContextPtr = std::unique_ptr<AVFormatContext, AVFormatContextDeleter>;
using AVCodecContextPtr = std::unique_ptr<AVCodecContext, AVCodecContextDeleter>;
using AVFramePtr = std::unique_ptr<AVFrame, AVFrameDeleter>;
using AVPacketPtr = std::unique_ptr<AVPacket, AVPacketDeleter>;
using SwsContextPtr = std::unique_ptr<SwsContext, SwsContextDeleter>;

// Helper to throw on error
inline void checkFFmpeg(int ret, const std::string& msg) {
    if (ret < 0) {
        char errbuf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(ret, errbuf, AV_ERROR_MAX_STRING_SIZE);
        throw std::runtime_error(msg + ": " + std::string(errbuf));
    }
}

inline AVFramePtr makeFrame() {
    AVFrame* frame = av_frame_alloc();
    if (!frame) throw std::runtime_error("Failed to allocate AVFrame");
    return AVFramePtr(frame);
}

inline AVPacketPtr makePacket() {
    AVPacket* pkt = av_packet_alloc();
    if (!pkt) throw std::runtime_error("Failed to allocate AVPacket");
    return AVPacketPtr(pkt);
}

} // namespace ve::core
