# FindFFmpeg.cmake
#
# Finds the FFmpeg libraries
#
# This module defines:
# FFmpeg_FOUND - True if FFmpeg was found
# FFmpeg_INCLUDE_DIRS - Include directories for FFmpeg
# FFmpeg_LIBRARIES - Libraries to link against
#
# And the following imported targets:
# FFmpeg::avcodec
# FFmpeg::avformat
# FFmpeg::avutil
# FFmpeg::swscale
# FFmpeg::swresample

include(FindPackageHandleStandardArgs)

function(find_ffmpeg_component component header library)
    find_path(FFmpeg_${component}_INCLUDE_DIR NAMES ${header}
        HINTS ${FFmpeg_ROOT}/include /usr/include /usr/local/include /opt/local/include
        PATH_SUFFIXES ffmpeg
    )

    find_library(FFmpeg_${component}_LIBRARY NAMES ${library}
        HINTS ${FFmpeg_ROOT}/lib /usr/lib /usr/local/lib /opt/local/lib
    )

    if(FFmpeg_${component}_INCLUDE_DIR AND FFmpeg_${component}_LIBRARY)
        set(FFmpeg_${component}_FOUND TRUE PARENT_SCOPE)
        set(FFmpeg_${component}_INCLUDE_DIRS ${FFmpeg_${component}_INCLUDE_DIR} PARENT_SCOPE)
        set(FFmpeg_${component}_LIBRARIES ${FFmpeg_${component}_LIBRARY} PARENT_SCOPE)

        if(NOT TARGET FFmpeg::${component})
            add_library(FFmpeg::${component} UNKNOWN IMPORTED)
            set_target_properties(FFmpeg::${component} PROPERTIES
                IMPORTED_LOCATION "${FFmpeg_${component}_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${FFmpeg_${component}_INCLUDE_DIR}"
            )
        endif()
    endif()
endfunction()

find_ffmpeg_component(avcodec libavcodec/avcodec.h avcodec)
find_ffmpeg_component(avformat libavformat/avformat.h avformat)
find_ffmpeg_component(avutil libavutil/avutil.h avutil)
find_ffmpeg_component(swscale libswscale/swscale.h swscale)
find_ffmpeg_component(swresample libswresample/swresample.h swresample)

set(FFmpeg_LIBRARIES
    ${FFmpeg_avcodec_LIBRARIES}
    ${FFmpeg_avformat_LIBRARIES}
    ${FFmpeg_avutil_LIBRARIES}
    ${FFmpeg_swscale_LIBRARIES}
    ${FFmpeg_swresample_LIBRARIES}
)

set(FFmpeg_INCLUDE_DIRS
    ${FFmpeg_avcodec_INCLUDE_DIRS}
    ${FFmpeg_avformat_INCLUDE_DIRS}
    ${FFmpeg_avutil_INCLUDE_DIRS}
    ${FFmpeg_swscale_INCLUDE_DIRS}
    ${FFmpeg_swresample_INCLUDE_DIRS}
)

find_package_handle_standard_args(FFmpeg
    REQUIRED_VARS FFmpeg_LIBRARIES FFmpeg_INCLUDE_DIRS
    HANDLE_COMPONENTS
)
