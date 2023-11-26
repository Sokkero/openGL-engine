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
        external/imgui/
        external/imgui/backends/
)

FILE(GLOB IMGUI
        external/imgui/*.cpp
        external/imgui/*.h
        external/imgui/backends/imgui_impl_glfw.h
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.h
        external/imgui/backends/imgui_impl_opengl3.cpp
        external/imgui/misc/cpp/imgui_stdlib.*
)

FILE(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${IMGUI} ${SOURCE_FILES} ${RES_FILES} main.cpp)

# Copy src/resources -> bin/resources
file(COPY ${CMAKE_SOURCE_DIR}/src/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
