IF EXIST build RMDIR /S /Q build
mkdir build && cd build
cmake -D CMAKE_PREFIX_PATH="%1" ..
devenv /build Release NASA_APOD.sln