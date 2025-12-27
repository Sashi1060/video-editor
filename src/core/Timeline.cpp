#include "Timeline.h"
#include <algorithm>

namespace ve::core {

void Timeline::addClip(const MediaClip& clip) {
    // Simple append logic for MVP
    MediaClip newClip = clip;
    if (clips_.empty()) {
        newClip.timelineStart = 0;
    } else {
        const auto& last = clips_.back();
        newClip.timelineStart = last.timelineStart + last.length();
    }
    clips_.push_back(newClip);
}

std::optional<MediaClip> Timeline::getClipAt(int64_t timestampUs) const {
    for (const auto& clip : clips_) {
        if (timestampUs >= clip.timelineStart && timestampUs < clip.timelineStart + clip.length()) {
            return clip;
        }
    }
    return std::nullopt;
}

int64_t Timeline::getDuration() const {
    if (clips_.empty()) return 0;
    const auto& last = clips_.back();
    return last.timelineStart + last.length();
}

} // namespace ve::core
