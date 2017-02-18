cmake_minimum_required(VERSION 3.6)
project(manuscrito)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES main.cpp Editor.cpp Editor.h Buffer.cpp Buffer.h)
add_executable(manuscrito ${SOURCE_FILES})


find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )
target_link_libraries(manuscrito ${CURSES_LIBRARIES} )

add_custom_target(run
        COMMAND manuscrito
        DEPENDS manuscrito
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        )