cmake_minimum_required(VERSION 3.10.0)

project (AudioMixerEngine)
set(CMAKE_CXX_STANDARD 17)
# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Essential include files to build a node addon,
# you should add this line in every CMake.js based project.
include_directories(${CMAKE_JS_INC})
# Declare the location of the source files
file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h")
# This line will tell CMake that we're building a shared library
# from the above source files
# named after the project's name
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
# This line will give our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.

include(FetchContent)

FetchContent_Declare(
  serialib
  GIT_REPOSITORY https://github.com/imabot2/serialib.git
  GIT_TAG        f7dbc73948ffc6a071822fe6e0775fdf87fa9f20
)
FetchContent_Populate(serialib)

target_include_directories(${PROJECT_NAME} PRIVATE ${serialib_SOURCE_DIR}/lib)
target_sources(${PROJECT_NAME} PRIVATE ${serialib_SOURCE_DIR}/lib/serialib.cpp)

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})
# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE
    NAPI_CPP_EXCEPTIONS
    NAPI_DISABLE_CPP_EXCEPTIONS=0
)

find_path(MINIAUDIO_INCLUDE_DIRS "miniaudio.h")
target_include_directories(AudioMixerEngine PRIVATE ${MINIAUDIO_INCLUDE_DIRS})
