###############################################
# libcurl 7.79.1 cmake module LIBCURL         #
# build on MACHINE x86 with MSVC10.0          #
# build by innoob(Jason Payne) in 2021-11-07  # 
# github: https://github.com/innoob/curl-msvc #
###############################################

FIND_PATH(LIBCURL_INCLUDE_DIR curl ${CMAKE_CURRENT_SOURCE_DIR}/libcurl/include)

FIND_LIBRARY(LIBCURL_LIBRARY libcurl.lib ${CMAKE_CURRENT_SOURCE_DIR}/libcurl/lib)

set(LIBCURL_FOUNDED FALSE)
if (LIBCURL_INCLUDE_DIR AND LIBCURL_LIBRARY)
	set(LIBCURL_FOUNDED TRUE)
endif()