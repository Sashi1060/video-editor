#include "PreviewWidget.h"
#include <QPainter>

namespace ve::ui {

PreviewWidget::PreviewWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(640, 360);
    setStyleSheet("background-color: black;");
}

void PreviewWidget::setFrame(std::shared_ptr<ve::core::RawFrame> frame) {
    if (!frame) {
        currentImage_ = QImage();
    } else {
        // Create QImage from raw data
        // Note: QImage does not take ownership of data by default, so we must copy
        // or ensure data persists. Since RawFrame is shared_ptr, we can copy.
        // For performance, we might want to optimize this later.
        QImage img(frame->data.data(), frame->width, frame->height, frame->linesize, QImage::Format_RGBA8888);
        currentImage_ = img.copy(); // Deep copy to detach from frame data
    }
    update();
}

void PreviewWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (!currentImage_.isNull()) {
        // Scale to fit while maintaining aspect ratio
        QImage scaled = currentImage_.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = (width() - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawImage(x, y, scaled);
    }
}

} // namespace ve::ui
