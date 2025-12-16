cmake_minimum_required(VERSION 3.10)
project(MiniOS)

# -----------------------------
# C++ Standard
# -----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Find Qt5 Widgets
# -----------------------------
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# -----------------------------
# Enable AUTOMOC, AUTOUIC, AUTORCC
# This ensures Q_OBJECT and .ui files are automatically handled
# -----------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -----------------------------
# Source files
# -----------------------------
set(SOURCES
    src/main.cpp
    src/ui/mainwindow.cpp
    src/process/process.cpp
    src/sync/sync.cpp
    src/memory/memory.cpp
    src/file/file.cpp
)

# -----------------------------
# Add executable
# -----------------------------
add_executable(MiniOS ${SOURCES})

# -----------------------------
# Include directories
# Must come AFTER add_executable
# -----------------------------
target_include_directories(MiniOS PRIVATE
    ${CMAKE_SOURCE_DIR}/src      # For your headers (mainwindow.h, process.h, etc.)
    ${CMAKE_BINARY_DIR}          # For generated files like ui_mainwindow.h
)

# -----------------------------
# Link Qt5 Widgets
# -----------------------------
target_link_libraries(MiniOS Qt5::Widgets)
