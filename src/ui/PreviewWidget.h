#pragma once

#include <QWidget>
#include <QImage>
#include <memory>
#include "core/MediaClip.h"

namespace ve::ui {

class PreviewWidget : public QWidget {
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    void setFrame(std::shared_ptr<ve::core::RawFrame> frame);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage currentImage_;
};

} // namespace ve::ui
