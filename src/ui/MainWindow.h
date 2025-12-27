#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QDoubleSpinBox>
#include "core/Engine.h"
#include "PreviewWidget.h"
#include "TimelineWidget.h"

namespace ve::ui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onImportClicked();
    void onExportClicked();
    void onPlayPauseClicked();
    void onTimerTick();
    void onTrimChanged();

private:
    void setupUi();
    void updateUiState();

    ve::core::Engine engine_;
    PreviewWidget* previewWidget_;
    TimelineWidget* timelineWidget_;
    
    QTimer* playbackTimer_;
    bool isPlaying_ = false;
    int64_t currentTimeUs_ = 0;

    // Controls
    QDoubleSpinBox* inPointSpin_;
    QDoubleSpinBox* outPointSpin_;
};

} // namespace ve::ui
