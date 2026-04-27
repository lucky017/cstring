
# lib/cmocka build options
set(WITH_EXAMPLES OFF CACHE BOOL "Do not build examples")
set(WITH_TESTING OFF CACHE BOOL "Do not build tests")
set(UNIT_TESTING ON CACHE BOOL "Enable unit testing")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../cmocka ${CMAKE_CURRENT_BINARY_DIR}/cmocka EXCLUDE_FROM_ALL)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cmocka/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

add_executable(run_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})

target_link_libraries(run_tests PRIVATE cstring cmocka-static)

add_custom_target(tests
    COMMAND ./run_tests
    DEPENDS run_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Building and running cstring tests..."
)
