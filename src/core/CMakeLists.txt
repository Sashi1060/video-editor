add_library(ve_core
    Engine.cpp
    Engine.h
    Logger.cpp
    Logger.h
    VideoDecoder.cpp
    VideoDecoder.h
    Timeline.cpp
    Timeline.h
    Exporter.cpp
    Exporter.h
)

target_include_directories(ve_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ve_core PUBLIC
    FFmpeg::avcodec
    FFmpeg::avformat
    FFmpeg::avutil
    FFmpeg::swscale
    FFmpeg::swresample
)
