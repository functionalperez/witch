cmake_minimum_required(VERSION 3.0.0)
project(witch CXX)

add_executable(witch witch.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

include_directories(include)
add_subdirectory(conduit)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(witch PRIVATE conduit Threads::Threads)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    file(GLOB all_tests "test/*.cpp")

    foreach(file_name ${all_tests})
        get_filename_component(test_name ${file_name} NAME_WE)
        add_executable("${test_name}" "${file_name}")
        target_link_libraries("${test_name}" PRIVATE conduit Threads::Threads)
        add_test(NAME "${test_name}" COMMAND "${test_name}")
    endforeach()
endif()


