cmake_minimum_required(VERSION 3.16)
project(SignalIOSystem LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Charts
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    MainInterface.ui
    MainInterface.h
    MainInterface.cpp
    SignalModel.h
    SignalModel.cpp
    SignalTimeDomainView.h
    SignalTimeDomainView.cpp
    SignalFreqDomainView.h
    SignalFreqDomainView.cpp
    NoiseGenerator.h
    NoiseGenerator.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Charts
)

