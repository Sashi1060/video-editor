# Verification Guide

## 1. Build
```bash
cd build
cmake ..
make
```

## 2. Run
```bash
./bin/video-editor
```

## 3. Manual Test Workflow
1.  **Import**: Click "Import Video" and select a test MP4 file (e.g., `BigBuckBunny.mp4`).
    *   *Expected*: Video loads, first frame appears in preview, blue bar appears in timeline.
2.  **Playback**: Click "Play/Pause".
    *   *Expected*: Video plays smoothly in the preview window. Red playhead moves in timeline.
3.  **Trim**:
    *   Set "In (s)" to `10.0`.
    *   Set "Out (s)" to `20.0`.
    *   *Expected*: Timeline bar shrinks visually. Playback loops between 10s and 20s.
4.  **Export**: Click "Export Selection" and save as `output.mp4`.
    *   *Expected*: Progress dialog appears and completes.
5.  **Verify Output**:
    ```bash
    ffprobe output.mp4
    ```
    *   *Expected*: Duration should be exactly 10 seconds. Video codec should be h264.

## 4. Known Limitations
*   **Audio**: Not supported yet (silent export).
*   **Performance**: Decoding is single-threaded on CPU. 4K video might lag.
*   **Formats**: Export is hardcoded to H.264/MP4.
*   **Trimming**: "In" point seek might be slightly inaccurate (keyframe dependent) for display, but export re-encodes so it should be frame-accurate relative to the seek point.
