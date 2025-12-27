#include "VideoDecoder.h"
#include "Logger.h"
#include <iostream>

namespace ve::core {

VideoDecoder::VideoDecoder() {
    frame_ = makeFrame();
    rgbFrame_ = makeFrame();
    packet_ = makePacket();
}

VideoDecoder::~VideoDecoder() {
}

void VideoDecoder::open(const std::string& filePath) {
    AVFormatContext* fmtCtx = nullptr;
    int ret = avformat_open_input(&fmtCtx, filePath.c_str(), nullptr, nullptr);
    checkFFmpeg(ret, "Failed to open input file");
    formatCtx_.reset(fmtCtx);

    ret = avformat_find_stream_info(formatCtx_.get(), nullptr);
    checkFFmpeg(ret, "Failed to find stream info");

    videoStreamIndex_ = -1;
    for (unsigned int i = 0; i < formatCtx_->nb_streams; i++) {
        if (formatCtx_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex_ = i;
            break;
        }
    }

    if (videoStreamIndex_ == -1) {
        throw std::runtime_error("No video stream found");
    }

    AVCodecParameters* codecPar = formatCtx_->streams[videoStreamIndex_]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecPar->codec_id);
    if (!codec) {
        throw std::runtime_error("Unsupported codec");
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        throw std::runtime_error("Failed to allocate codec context");
    }
    codecCtx_.reset(codecCtx);

    ret = avcodec_parameters_to_context(codecCtx_.get(), codecPar);
    checkFFmpeg(ret, "Failed to copy codec params");

    ret = avcodec_open2(codecCtx_.get(), codec, nullptr);
    checkFFmpeg(ret, "Failed to open codec");

    // Fill clip info
    clipInfo_.filePath = filePath;
    clipInfo_.duration = formatCtx_->duration * 1000000 / AV_TIME_BASE; // to microseconds
    if (clipInfo_.duration < 0) clipInfo_.duration = 0; // Handle unknown duration
    
    clipInfo_.width = codecCtx_->width;
    clipInfo_.height = codecCtx_->height;
    
    if (formatCtx_->streams[videoStreamIndex_]->avg_frame_rate.den > 0) {
        clipInfo_.fps = av_q2d(formatCtx_->streams[videoStreamIndex_]->avg_frame_rate);
    } else {
        clipInfo_.fps = 30.0; // Fallback
    }

    clipInfo_.inPoint = 0;
    clipInfo_.outPoint = clipInfo_.duration;

    Logger::info("Opened video: " + filePath);
    Logger::info("Duration: " + std::to_string(clipInfo_.duration) + " us");
}

void VideoDecoder::initSwsContext(int width, int height, AVPixelFormat format) {
    if (!swsCtx_ || codecCtx_->width != width || codecCtx_->height != height) {
        swsCtx_.reset(sws_getContext(
            width, height, format,
            width, height, AV_PIX_FMT_RGBA,
            SWS_BILINEAR, nullptr, nullptr, nullptr
        ));
        
        // Allocate buffer for RGB frame
        int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, width, height, 1);
        rgbBuffer_.resize(numBytes);
        
        av_image_fill_arrays(rgbFrame_->data, rgbFrame_->linesize, rgbBuffer_.data(),
                             AV_PIX_FMT_RGBA, width, height, 1);
    }
}

bool VideoDecoder::seek(int64_t timestampUs) {
    // Convert to stream time base
    int64_t timestamp = av_rescale_q(timestampUs, {1, 1000000}, formatCtx_->streams[videoStreamIndex_]->time_base);
    int ret = av_seek_frame(formatCtx_.get(), videoStreamIndex_, timestamp, AVSEEK_FLAG_BACKWARD);
    if (ret < 0) return false;
    
    avcodec_flush_buffers(codecCtx_.get());
    return true;
}

std::shared_ptr<RawFrame> VideoDecoder::decodeNextFrame() {
    while (av_read_frame(formatCtx_.get(), packet_.get()) >= 0) {
        if (packet_->stream_index == videoStreamIndex_) {
            int ret = avcodec_send_packet(codecCtx_.get(), packet_.get());
            if (ret < 0) {
                av_packet_unref(packet_.get());
                continue; 
            }

            ret = avcodec_receive_frame(codecCtx_.get(), frame_.get());
            if (ret == 0) {
                // We have a frame
                initSwsContext(frame_->width, frame_->height, (AVPixelFormat)frame_->format);
                
                sws_scale(swsCtx_.get(),
                          frame_->data, frame_->linesize, 0, frame_->height,
                          rgbFrame_->data, rgbFrame_->linesize);
                
                auto raw = std::make_shared<RawFrame>();
                raw->width = frame_->width;
                raw->height = frame_->height;
                raw->linesize = rgbFrame_->linesize[0];
                raw->data = rgbBuffer_; // Copy data
                
                // PTS calculation
                if (frame_->pts != AV_NOPTS_VALUE) {
                     raw->pts = av_rescale_q(frame_->pts, formatCtx_->streams[videoStreamIndex_]->time_base, {1, 1000000});
                } else {
                    raw->pts = 0;
                }

                av_packet_unref(packet_.get());
                return raw;
            }
        }
        av_packet_unref(packet_.get());
    }
    return nullptr; // EOF or error
}

std::shared_ptr<RawFrame> VideoDecoder::getFrameAt(int64_t timestampUs) {
    if (!seek(timestampUs)) return nullptr;
    
    // Seek lands on a keyframe before the target.
    // We must decode frames until we reach the target timestamp.
    
    while (true) {
        auto frame = decodeNextFrame();
        if (!frame) return nullptr; // End of stream or error
        
        // If frame PTS is close enough or past the target, return it.
        // Tolerance: 1 frame duration (approx 33ms) or just >= target
        if (frame->pts >= timestampUs) {
            return frame;
        }
        
        // If we are too far behind (e.g. > 1 sec), maybe seek failed or we are lost?
        // But normally we just keep decoding.
    }
}

} // namespace ve::core
