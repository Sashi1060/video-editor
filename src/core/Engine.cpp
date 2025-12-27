#include "Engine.h"
#include "Logger.h"

namespace ve::core {

Engine::Engine() {
}

Engine::~Engine() {
}

void Engine::initialize() {
    Logger::info("Core Engine Initialized");
}

void Engine::addClip(const std::string& filePath) {
    try {
        // Probe file to get duration/metadata
        VideoDecoder tempDecoder;
        tempDecoder.open(filePath);
        timeline_.addClip(tempDecoder.getClipInfo());
        Logger::info("Added clip: " + filePath);
    } catch (const std::exception& e) {
        Logger::error("Failed to add clip: " + std::string(e.what()));
    }
}

VideoDecoder* Engine::getDecoder(const std::string& filePath) {
    if (decoders_.find(filePath) == decoders_.end()) {
        auto decoder = std::make_unique<VideoDecoder>();
        try {
            decoder->open(filePath);
            decoders_[filePath] = std::move(decoder);
        } catch (const std::exception& e) {
            Logger::error("Failed to open decoder for: " + filePath);
            return nullptr;
        }
    }
    return decoders_[filePath].get();
}

std::shared_ptr<RawFrame> Engine::render(int64_t timestampUs) {
    auto clipOpt = timeline_.getClipAt(timestampUs);
    if (!clipOpt) {
        return nullptr; // No clip at this time (black frame in UI)
    }
    
    const auto& clip = *clipOpt;
    auto* decoder = getDecoder(clip.filePath);
    if (!decoder) return nullptr;
    
    int64_t localTime = timestampUs - clip.timelineStart + clip.inPoint;
    return decoder->getFrameAt(localTime);
}

} // namespace ve::core
