
cmake_minimum_required(VERSION 4.3.2)

project(object C)
add_library(cstring SHARED cstring.c)

target_include_directories(cstring PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

add_executable(${PROJECT_NAME} Main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE cstring)

add_custom_target(run
    COMMAND ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running ${PROJECT_NAME}..."
)



