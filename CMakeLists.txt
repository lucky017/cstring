
cmake_minimum_required(VERSION 4.3.2)

project(object C)

# Default is OFF (Static). To build Shared, 
# use: cmake -DBUILD_SHARED=ON ..
option(BUILD_SHARED "Build cstring as a shared library" OFF)
option(BUILD_TESTS "Build tests for cstring" OFF)

if(BUILD_SHARED)
    set(LIB_TYPE SHARED)
    message(STATUS "Building cstring as a SHARED library")
else()
    set(LIB_TYPE STATIC)
    message(STATUS "Building cstring as a STATIC library")
endif()

add_library(cstring ${LIB_TYPE} cstring.c)

target_include_directories(cstring PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_executable(${PROJECT_NAME} Main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE cstring)

add_custom_target(run
    COMMAND ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running ${PROJECT_NAME}..."
)



