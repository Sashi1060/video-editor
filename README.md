# Video Editor (Open Source)

A simple, performant, and ethical video editor for Linux.

## Philosophy
- No watermark
- No login
- No cloud
- No AI gimmicks
- Offline-first

## Building

### Prerequisites
- CMake 3.16+
- Qt 6
- FFmpeg (libavcodec, libavformat, libavutil, libswscale)
- C++20 compiler

### Build Steps
```bash
mkdir build
cd build
cmake ..
make
./bin/video-editor
```
