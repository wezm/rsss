PROJECT(RSSS C)

cmake_minimum_required(VERSION 2.8)

INCLUDE(FindLibXml2)

ENABLE_TESTING()

# set(CMAKE_C_FLAGS_INIT -Wall -Werror "-Wno-error=\#warnings")

ADD_LIBRARY(
  rsss
SHARED
  entries.c
  subscriptions.c
  feeds.c
)
ADD_EXECUTABLE(rsssd rsssd.c)
ADD_EXECUTABLE(check_rsss
  test/check_subscriptions.c
  test/check_rsss.c
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(libmicrohttpd REQUIRED)
include_directories(${LIBMICROHTTPD_INCLUDE_DIRS})
set(LIBS ${LIBS} ${LIBMICROHTTPD_LIBRARIES})

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
set(LIBS ${LIBS} ${LIBXML2_LIBRARIES})

# This should be optional...
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(rsssd rsss)
# TARGET_LINK_LIBRARIES(backpages backpage ${CURL_LIBRARIES})
TARGET_LINK_LIBRARIES(rsss ${LIBS})

# Testing, this should only be generated if Check was found
TARGET_LINK_LIBRARIES(check_rsss rsss ${CHECK_LIBRARIES})

ADD_TEST(librsss ${CMAKE_CURRENT_BINARY_DIR}/check_rsss)
