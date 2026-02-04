cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "openGLEngine")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g -O0")
set(CONAN_DISABLE_CHECK_COMPILER ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")

enable_testing()

project(${PROJECT_NAME})

find_package(OpenGL REQUIRED)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/conanBuild/conanbuildinfo.cmake)
    message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run ./build.sh first")
endif()

include(${CMAKE_BINARY_DIR}/conanBuild/conanbuildinfo.cmake)
conan_basic_setup()

FILE(GLOB IMGUI
        src/external/implot/*.cpp
        src/external/implot/*.h
        src/external/imgui/*.cpp
        src/external/imgui/*.h
        src/external/imgui/backends/imgui_impl_glfw.h
        src/external/imgui/backends/imgui_impl_glfw.cpp
        src/external/imgui/backends/imgui_impl_opengl3.h
        src/external/imgui/backends/imgui_impl_opengl3.cpp
        src/external/imgui/misc/cpp/imgui_stdlib.*
)
FILE(GLOB_RECURSE SOURCE_FILES src/internal/*.cpp src/internal/*.h)

add_executable(${PROJECT_NAME} ${IMGUI} ${SOURCE_FILES} ${RES_FILES} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
        src/external/imgui/
        src/external/imgui/backends/
        src/external/implot/
        src/internal/
)

FILE(GLOB GLOBAL_DEFINITIONS src/internal/utils/globalDefinitions/*.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${GLOBAL_DEFINITIONS})

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Copy src/resources -> bin/resources
file(COPY ${CMAKE_SOURCE_DIR}/src/internal/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
