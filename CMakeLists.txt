cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "openGLEngine")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")

project(${PROJECT_NAME})

find_package(OpenGL REQUIRED)

if(EXISTS ${CMAKE_BINARY_DIR}/conanBuild/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanBuild/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run ./build.sh first")
endif()

include_directories(
        external/SOIL/
)

FILE(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RES_FILES} main.cpp
        src/resources/shader/SolidTextureShader.cpp
        src/resources/shader/SolidTextureShader.h)

# Copy src/resources -> bin/resources
file(COPY ${CMAKE_SOURCE_DIR}/src/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} ${CMAKE_SOURCE_DIR}/external/assimp/libassimp.5.2.5.dylib)
