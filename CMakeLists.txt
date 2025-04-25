cmake_minimum_required(VERSION 3.14)
project(WhatYouFeelin)

set(CMAKE_CXX_STANDARD 17)

# Define OpenGL libraries variable
set(OPENGL_LIBRARIES "")

# Find OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Check if OpenGL was found
if (OPENGL_FOUND)
    message(STATUS "OpenGL found: ${OPENGL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenGL not found")
endif()

include_directories("${CMAKE_SOURCE_DIR}/include")

# Add Glad, GLFW, and GLM directories
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

# === OpenCV as ExternalProject ===
include(ExternalProject)

# Define OpenCV installation path
set(OpenCV_INSTALL_DIR ${CMAKE_BINARY_DIR}/opencv-install)

ExternalProject_Add(OpenCVProject
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/opencv
    CMAKE_ARGS
        -G "Unix Makefiles" # Use Unix Makefiles for MinGW
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_SHARED_LIBS=OFF
        -DCMAKE_INSTALL_PREFIX=${OpenCV_INSTALL_DIR}
        -DWITH_IPP=OFF
        -DWITH_TBB=OFF
        -DWITH_OPENMP=OFF
        -DWITH_CUDA=OFF
        -DWITH_MSMF=OFF
        -DWITH_VFW=OFF
        -DWITH_QT=OFF
        -DWITH_OPENGL=ON
        -DWITH_DSHOW=ON
        -DBUILD_opencv_world=ON
        -DBUILD_TESTS=OFF
        -DBUILD_PERF_TESTS=OFF
        -DBUILD_EXAMPLES=OFF
        -DBUILD_DOCS=OFF
        -DBUILD_JAVA=OFF
        -DBUILD_opencv_highgui=ON
    INSTALL_DIR ${OpenCV_INSTALL_DIR}
)

# Include and link OpenCV
set(OpenCV_INCLUDE_DIR ${OpenCV_INSTALL_DIR}/include)
set(OpenCV_LIB_DIR ${OpenCV_INSTALL_DIR}/x64/mingw/staticlib)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# Create executable
add_executable(WhatYouFeelin src/main.cpp src/render.cpp src/input.cpp)

# Ensure OpenCV builds first
add_dependencies(WhatYouFeelin OpenCVProject)

include_directories(${OpenCV_INCLUDE_DIR})

# Link OpenCV libraries (static .a files)
target_link_libraries(WhatYouFeelin PRIVATE
    ${OpenCV_LIB_DIR}/libopencv_world4110.a
    ${OpenCV_LIB_DIR}/libade.a
    ${OpenCV_LIB_DIR}/liblibjpeg-turbo.a
    ${OpenCV_LIB_DIR}/liblibpng.a
    ${OpenCV_LIB_DIR}/liblibtiff.a
    ${OpenCV_LIB_DIR}/liblibwebp.a
    ${OpenCV_LIB_DIR}/liblibopenjp2.a
    ${OpenCV_LIB_DIR}/liblibprotobuf.a
    ${OpenCV_LIB_DIR}/libIlmImf.a
    ${OpenCV_LIB_DIR}/libzlib.a

    # System libraries (Windows)
    opengl32
    gdi32
    user32
    ws2_32
    ole32
    setupapi
    uuid
)

# Link OpenGL libraries
target_link_libraries(WhatYouFeelin PRIVATE glad glfw glm ${OPENGL_LIBRARIES})

# Copy shaders to output
file(GLOB Shaders
    "${CMAKE_SOURCE_DIR}/shaders/*"
)
file(COPY ${Shaders}
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
