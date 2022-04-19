# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(PROJNAME "NASA_APOD")
set(APIHANDLER "APIHANDLER")
set(RSC_DIR ${CMAKE_SOURCE_DIR}/icons)

set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.12.12\\5.12.12\\msvc2017_64")
set(PROG_DIR ${CMAKE_SOURCE_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_USE_QTSVG 1)

project (${PROJNAME})

find_package(Qt5Widgets)
find_package(Qt5Svg REQUIRED)
include(FeatureSummary)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# Include sub-projects.
add_subdirectory ("src")
