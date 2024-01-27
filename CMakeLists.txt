cmake_minimum_required(VERSION 3.10)
project(homeassistant-websocket-api-helper)

file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE sources_test src/curl_helper.cpp src/util.cpp src/web_socket.cpp test/src/*.cpp)

# Add the sources to the target
add_executable(homeassistant-websocket-api-helper ${sources})

set(APP_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)

set_property(TARGET homeassistant-websocket-api-helper PROPERTY INCLUDE_DIRECTORIES ${APP_INCLUDE_DIRECTORIES})

# Debug build
SET(CMAKE_BUILD_TYPE Debug)
ADD_DEFINITIONS("-DDEBUG")

# Release build
#SET(CMAKE_BUILD_TYPE Release)
#ADD_DEFINITIONS("-DNDEBUG")

# Select flags
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
SET(CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_LINKER_FLAGS_RELEASE}")
SET(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG}")

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_RELEASE}")
ELSE()
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG}")
ENDIF()

IF(UNIX)
  # Add definitions, compiler switches, etc.
  SET(COMPILER_COMMON_FLAGS "-std=c++20 -Wall -W -Wextra -Werror -Wformat -Wformat-y2k -Winit-self -Wstack-protector -Wunknown-pragmas -Wundef -Wwrite-strings -Wno-unused-parameter -Wpointer-arith -Wno-switch -Woverloaded-virtual -Wno-stack-protector -Wmissing-include-dirs -Wuninitialized -O2")

  SET(COMPILER_PLATFORM_DEFINES "-D__LINUX__")
ENDIF()

IF(WIN32)
  SET(COMPILER_PLATFORM_DEFINES "-DUNICODE -D_UNICODE -DNOMINMAX")
ENDIF()

target_compile_options(homeassistant-websocket-api-helper PUBLIC "${COMPILER_PLATFORM_DEFINES} ${COMPILER_COMMON_FLAGS}")


###############################################################################
## dependencies ###############################################################
###############################################################################

# System curl library
#set(CURL_LIBRARY "-lcurl")
#find_package(CURL REQUIRED)

# Our curl library with websockets enabled
set(CURL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/output/include")
set(CURL_LIB_DIR "${PROJECT_SOURCE_DIR}/output/lib")

target_include_directories(homeassistant-websocket-api-helper SYSTEM PUBLIC ${CURL_INCLUDE_DIR})
target_link_directories(homeassistant-websocket-api-helper PUBLIC ${CURL_LIB_DIR})
target_link_libraries(homeassistant-websocket-api-helper PUBLIC curl)


###############################################################################
## testing ####################################################################
###############################################################################

# this is for our testing framework
# we don't add REQUIRED because it's just for testing
find_package(GTest)

add_executable(unit_tests ${sources_test})

# we add this define to prevent collision with the main
# this might be better solved by not adding the source with the main to the
# testing target
target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

# this allows us to use our executable as a link library
# therefore we can inherit all compiler options and library dependencies
set_target_properties(homeassistant-websocket-api-helper PROPERTIES ENABLE_EXPORTS on)

set_property(TARGET unit_tests PROPERTY INCLUDE_DIRECTORIES ${APP_INCLUDE_DIRECTORIES})

target_include_directories(unit_tests SYSTEM PUBLIC ${CURL_INCLUDE_DIR})
target_link_directories(unit_tests PUBLIC ${CURL_LIB_DIR})

target_link_libraries(unit_tests PUBLIC
  ${GTEST_BOTH_LIBRARIES}
  curl
)

target_include_directories(unit_tests PUBLIC
  ${GTEST_INCLUDE_DIRS} # doesn't do anything on Linux
)
