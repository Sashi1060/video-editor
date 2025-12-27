add_library(ve_ui
    MainWindow.cpp
    MainWindow.h
    PreviewWidget.cpp
    PreviewWidget.h
    TimelineWidget.cpp
    TimelineWidget.h
)

target_include_directories(ve_ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ve_ui PUBLIC
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    ve_core
)
