# Add the source files
add_library(Glad glad.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)