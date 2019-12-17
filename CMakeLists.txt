cmake_minimum_required(VERSION 3.5)

project(LegoDetector LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )

add_executable(LegoDetector main.cpp)
target_link_libraries(LegoDetector ${OpenCV_LIBS})
