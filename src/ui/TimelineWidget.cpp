#include "TimelineWidget.h"
#include <QPainter>

namespace ve::ui {

TimelineWidget::TimelineWidget(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(100);
    setStyleSheet("background-color: #333;");
}

void TimelineWidget::setTimeline(ve::core::Timeline* timeline) {
    timeline_ = timeline;
    update();
}

void TimelineWidget::setCurrentTime(int64_t timeUs) {
    currentTimeUs_ = timeUs;
    update();
}

void TimelineWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor(50, 50, 50));

    if (!timeline_) return;

    // Draw clips
    int64_t totalDuration = timeline_->getDuration();
    if (totalDuration == 0) totalDuration = 10000000; // Default 10s view

    double pixelsPerUs = (double)width() / totalDuration;

    for (const auto& clip : timeline_->getClips()) {
        int x = clip.timelineStart * pixelsPerUs;
        int w = clip.length() * pixelsPerUs;
        painter.fillRect(x, 10, w, height() - 20, QColor(100, 150, 200));
        painter.setPen(Qt::white);
        painter.drawText(x + 5, 30, QString::fromStdString(clip.filePath));
    }

    // Draw playhead
    int playheadX = currentTimeUs_ * pixelsPerUs;
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(playheadX, 0, playheadX, height());
}

} // namespace ve::ui
