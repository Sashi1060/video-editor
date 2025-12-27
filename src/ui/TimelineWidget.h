#pragma once

#include <QWidget>
#include "core/Timeline.h"

namespace ve::ui {

class TimelineWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    void setTimeline(ve::core::Timeline* timeline);
    void setCurrentTime(int64_t timeUs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    ve::core::Timeline* timeline_ = nullptr;
    int64_t currentTimeUs_ = 0;
};

} // namespace ve::ui
