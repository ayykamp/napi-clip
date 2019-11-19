cmake_minimum_required(VERSION 3.2)

# Name of the project (will be the name of the plugin)
project (napi_clip)

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  # As we compile with CMAKE_OSX_DEPLOYMENT_TARGET=10.7, we have to
  # explicitly say that we want to use libc++ instead of the GNU libstdc++
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# lets find a better way :)))
set (CMAKE_CXX_FLAGS "-fPIC")
add_compile_definitions(NAPI_CPP_EXCEPTIONS)

# Essential include files to build a node addon,
# you should add this line in every CMake.js based project.
include_directories(${CMAKE_JS_INC})
add_subdirectory(cpp_modules/clip)

# Declare the location of the source files
file(GLOB SOURCE_FILES "src/*.cpp")

# This line will tell CMake that we're building a shared library
# from the above source files
# named after the project's name
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# This line will give our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.
target_link_libraries(${PROJECT_NAME} clip)
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api")
