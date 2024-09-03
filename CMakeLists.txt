cmake_minimum_required(VERSION 3.28)
project(minesweaper)

set(CMAKE_CXX_STANDARD 17)

# Set the path to your SFML directory
# Update this path to where you have extracted SFML
set(SFML_DIR "D:/projekti/SFML-2.6.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(minesweaper main.cpp)

target_link_libraries(minesweaper sfml-graphics sfml-window sfml-system)
