cmake_minimum_required(VERSION 3.5)

project(nexign_task1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 17)

file(GLOB headers "*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
source_group("Header Files" FILES ${headers})

set(sources
    storage.cpp
    solution.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${sources} ${headers})
