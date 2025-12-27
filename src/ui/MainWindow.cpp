#include "MainWindow.h"
#include <QApplication>
#include "core/Logger.h"
#include "core/Exporter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QProgressDialog>

namespace ve::ui {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    engine_.initialize();
    setupUi();

    playbackTimer_ = new QTimer(this);
    playbackTimer_->setInterval(33); // ~30 FPS
    connect(playbackTimer_, &QTimer::timeout, this, &MainWindow::onTimerTick);
    
    resize(1280, 800);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUi() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Top: Preview
    previewWidget_ = new PreviewWidget(this);
    mainLayout->addWidget(previewWidget_, 1);

    // Controls
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    
    QPushButton* importBtn = new QPushButton("Import Video", this);
    connect(importBtn, &QPushButton::clicked, this, &MainWindow::onImportClicked);
    controlsLayout->addWidget(importBtn);

    QPushButton* playBtn = new QPushButton("Play/Pause", this);
    connect(playBtn, &QPushButton::clicked, this, &MainWindow::onPlayPauseClicked);
    controlsLayout->addWidget(playBtn);

    controlsLayout->addWidget(new QLabel("In (s):"));
    inPointSpin_ = new QDoubleSpinBox(this);
    inPointSpin_->setRange(0, 99999);
    connect(inPointSpin_, &QDoubleSpinBox::valueChanged, this, &MainWindow::onTrimChanged);
    controlsLayout->addWidget(inPointSpin_);

    controlsLayout->addWidget(new QLabel("Out (s):"));
    outPointSpin_ = new QDoubleSpinBox(this);
    outPointSpin_->setRange(0, 99999);
    connect(outPointSpin_, &QDoubleSpinBox::valueChanged, this, &MainWindow::onTrimChanged);
    controlsLayout->addWidget(outPointSpin_);

    QPushButton* exportBtn = new QPushButton("Export Selection", this);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::onExportClicked);
    controlsLayout->addWidget(exportBtn);

    mainLayout->addLayout(controlsLayout);

    // Bottom: Timeline
    timelineWidget_ = new TimelineWidget(this);
    timelineWidget_->setTimeline(&engine_.getTimeline());
    mainLayout->addWidget(timelineWidget_);
}

void MainWindow::onImportClicked() {
    QString path = QFileDialog::getOpenFileName(this, "Import Video", "", "Video Files (*.mp4 *.mkv *.avi *.mov)");
    if (!path.isEmpty()) {
        engine_.addClip(path.toStdString());
        
        // Auto-set limits based on first clip
        auto& clips = engine_.getTimeline().getClips();
        if (!clips.empty()) {
            double durationSec = clips[0].duration / 1000000.0;
            outPointSpin_->setValue(durationSec);
            clips[0].outPoint = clips[0].duration; // Ensure clip has full duration initially
        }
        
        timelineWidget_->update();
        onTimerTick(); // Render first frame
    }
}

void MainWindow::onPlayPauseClicked() {
    isPlaying_ = !isPlaying_;
    if (isPlaying_) {
        playbackTimer_->start();
    } else {
        playbackTimer_->stop();
    }
}

void MainWindow::onTimerTick() {
    if (isPlaying_) {
        currentTimeUs_ += 33333; // Advance time
        
        // Loop logic for MVP (loop within trim range of first clip)
        auto& clips = engine_.getTimeline().getClips();
        if (!clips.empty()) {
            int64_t endUs = clips[0].timelineStart + clips[0].length();
            if (currentTimeUs_ >= endUs) {
                currentTimeUs_ = clips[0].timelineStart; // Loop back
            }
        }
    }

    auto frame = engine_.render(currentTimeUs_);
    previewWidget_->setFrame(frame);
    timelineWidget_->setCurrentTime(currentTimeUs_);
}

void MainWindow::onTrimChanged() {
    // Update first clip's in/out points
    auto& clips = engine_.getTimeline().getClips();
    if (!clips.empty()) {
        clips[0].inPoint = (int64_t)(inPointSpin_->value() * 1000000.0);
        clips[0].outPoint = (int64_t)(outPointSpin_->value() * 1000000.0);
        
        // Clamp
        if (clips[0].inPoint < 0) clips[0].inPoint = 0;
        if (clips[0].outPoint > clips[0].duration) clips[0].outPoint = clips[0].duration;
        if (clips[0].inPoint >= clips[0].outPoint) clips[0].inPoint = clips[0].outPoint - 100000; // Min 100ms

        timelineWidget_->update();
        
        // Seek to in-point
        currentTimeUs_ = clips[0].timelineStart;
        onTimerTick();
    }
}

void MainWindow::onExportClicked() {
    auto& clips = engine_.getTimeline().getClips();
    if (clips.empty()) return;

    QString path = QFileDialog::getSaveFileName(this, "Export Video", "output.mp4", "MP4 Files (*.mp4)");
    if (path.isEmpty()) return;

    QProgressDialog progress("Exporting...", "Cancel", 0, 100, this);
    progress.setWindowModality(Qt::WindowModal);
    
    try {
        ve::core::Exporter::exportTrim(
            clips[0].filePath,
            path.toStdString(),
            clips[0].inPoint,
            clips[0].outPoint,
            [&](float p) {
                progress.setValue((int)(p * 100));
                QApplication::processEvents();
            }
        );
        QMessageBox::information(this, "Success", "Export completed successfully!");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Export Failed", e.what());
    }
}

} // namespace ve::ui
