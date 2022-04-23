# CMakeList.txt : CMake project for qt-nasa-systray, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_library(${APIHANDLER} STATIC src/ApiHandlerImpl.cpp IApiHandler.cpp "inc/jsmn.h" "src/ApiParser.cpp" "inc/ApiParser.h")
target_include_directories(${APIHANDLER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${LIBCURL_INCLUDE_DIR})
target_link_libraries(${APIHANDLER} PRIVATE ${LIBCURL_LIBRARY})