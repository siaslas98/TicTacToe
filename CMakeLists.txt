cmake_minimum_required(VERSION 3.28)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)

add_executable(TicTacToe main.cpp
)

# Find SFML package
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Include SFML headers
include_directories(${SFML_INCLUDE_DIR})

# Link SFML libraries
target_link_libraries(TicTacToe sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
