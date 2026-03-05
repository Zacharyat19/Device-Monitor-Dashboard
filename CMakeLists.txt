cmake_minimum_required(VERSION 3.16)
project(DeviceMonitorDashboard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

# Explicitly add the headers from the Inc folder so AUTOMOC sees them
add_executable(dashboard
    main.cpp
    Src/Sensor.cpp
    Src/DashboardWindow.cpp
    Inc/Sensor.h
    Inc/DashboardWindow.h
)

target_include_directories(dashboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Inc)

target_link_libraries(dashboard PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::Charts
)