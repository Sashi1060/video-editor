# Video Editor MVP - Walkthrough

Welcome to your new open-source video editor! This guide will help you get started.

## 1. Launching the App
Open a terminal and run:
```bash
cd /home/trilochan-sashank/Desktop/video-editor/build
./bin/video-editor
```

## 2. The Interface
*   **Preview Window**: The large black box at the top. Shows your video.
*   **Controls**:
    *   **Import Video**: Load a file.
    *   **Play/Pause**: Toggle playback.
    *   **In (s) / Out (s)**: Set the start and end time for your trim.
    *   **Export Selection**: Save the trimmed video.
*   **Timeline**: The bar at the bottom. Shows the clip name and a red playhead.

## 3. Step-by-Step Guide: Trimming a Video
1.  **Import**: Click **Import Video**. Choose an MP4 file.
    *   *Tip*: The video will appear in the preview.
2.  **Preview**: Click **Play/Pause** to watch the video.
    *   *Note*: The red line on the timeline moves as it plays.
3.  **Set Trim Points**:
    *   Watch the video and note the time you want to start.
    *   Enter that time (in seconds) in the **In (s)** box.
    *   Enter the end time in the **Out (s)** box.
    *   *Visual Feedback*: The blue bar on the timeline will shrink to show your selection.
    *   *Looping*: Playback will now loop between your In and Out points so you can fine-tune the cut.
4.  **Export**:
    *   Click **Export Selection**.
    *   Choose a filename (e.g., `my_trim.mp4`).
    *   Wait for the progress bar to finish.
    *   *Done!* Your new video is saved without re-encoding the whole thing (it's fast!).

## 4. Troubleshooting
*   **"Export Failed"**: Ensure you have write permissions to the destination folder.
*   **Laggy Preview**: 4K videos might be slow on older CPUs. This is a known limitation of the MVP.
*   **No Audio**: This version is video-only. Audio support is coming in Milestone 2.

## 5. Development Info
*   **Source Code**: `/home/trilochan-sashank/Desktop/video-editor/src`
*   **Build System**: CMake + Make
*   **Dependencies**: Qt 6, FFmpeg
