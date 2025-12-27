add_executable(video-editor
    main.cpp
)

target_link_libraries(video-editor PRIVATE
    ve_core
    ve_ui
    Qt6::Widgets
    Qt6::Core
)
