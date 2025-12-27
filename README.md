# 🎬 Video Editor (Open Source, Linux)

# Overview

A lightweight, fast, and ethical video editor built for Linux.  
This project is an MVP video editor focused on **core video trimming workflows**, without unnecessary bloat, accounts, or cloud dependency.

---

# Philosophy

- No watermark
- No login
- No cloud
- No AI gimmicks
- Offline-first

This project deliberately avoids modern video-editor bloat and prioritizes **performance, transparency, and user control**.

---

# Features (v1 / MVP)

- Import local MP4 video files
- Video preview with play / pause
- Set trim points using **In (seconds)** and **Out (seconds)**
- Loop playback between trim points for precision
- Timeline visualization with playhead
- Export trimmed video **without full re-encoding** (fast FFmpeg-based export)
- Clean and distraction-free Qt UI

> Note: Audio support is planned for a future milestone.

---

# Tech Stack

- **Language**: C++20
- **UI Framework**: Qt 6
- **Video Processing**: FFmpeg
- **Build System**: CMake
- **Platform**: Linux

---

# Prerequisites

Ensure the following dependencies are installed:

- CMake 3.16 or newer
- Qt 6
- FFmpeg libraries:
  - libavcodec
  - libavformat
  - libavutil
  - libswscale
- C++20 compatible compiler (GCC or Clang)

---

# Build Instructions

```bash
mkdir build
cd build
cmake ..
make
./bin/video-editor
```
