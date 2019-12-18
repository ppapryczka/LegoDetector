cmake_minimum_required(VERSION 3.5)

project(LegoDetector LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )

set( SOURCE_FILES
    src/utils.hpp
    src/PixelPicker.hpp
    src/PixelPicker.cpp
    )

set( TEST_FILES
    tests/catch2.hpp
    tests/test_utils.cpp
    tests/test_main.cpp
    )


add_executable(LegoDetector src/main.cpp ${SOURCE_FILES} )
add_executable(LegoDetector_tests ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(LegoDetector ${OpenCV_LIBS})
target_link_libraries(LegoDetector_tests ${OpenCV_LIBS})
