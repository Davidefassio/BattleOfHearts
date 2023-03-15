cmake_minimum_required(VERSION 3.12)

# set the project name
project(BattleOfHearts)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify compiler options
if (MSVC)
    # warning level 4
    add_compile_options(/O3)
else()
    # additional warnings
    add_compile_options(-O3)
endif()

# add headers and executable
include_directories(${PROJECT_SOURCE_DIR})

add_executable(BattleOfHearts 
    src/BattleOfHearts.cpp
    lib/tiny-process/process.cpp
    lib/tiny-process/process_win.cpp
    lib/tiny-process/process_unix.cpp
    )