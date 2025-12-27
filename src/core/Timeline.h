#pragma once

#include "MediaClip.h"
#include <vector>
#include <optional>

namespace ve::core {

class Timeline {
public:
    void addClip(const MediaClip& clip);
    std::optional<MediaClip> getClipAt(int64_t timestampUs) const;
    int64_t getDuration() const;
    
    // For MVP, just expose clips to UI
    std::vector<MediaClip>& getClips() { return clips_; }

private:
    std::vector<MediaClip> clips_;
};

} // namespace ve::core
