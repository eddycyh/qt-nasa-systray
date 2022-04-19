# CMakeList.txt : CMake project for qt-nasa-systray, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (${PROJNAME} WIN32 "main.cpp" "window.cpp" "${PROG_DIR}/inc/window.h" ${RSC_DIR}/systray.qrc)
target_include_directories(${PROJNAME} PUBLIC ${PROG_DIR}/inc ${API_DIR} ${API_DIR}/inc)
target_link_libraries(${PROJNAME} Qt5::Widgets Qt5::Svg ${APIHANDLER})