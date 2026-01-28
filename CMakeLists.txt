cmake_minimum_required(VERSION 3.10)
project(BlackjackAI)

set(CMAKE_CXX_STANDARD 17)

# 1. Find OpenCV
find_package(OpenCV REQUIRED)

# 2. Find SQLite (Using pkg-config)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# 3. Include directories
include_directories(${OpenCV_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIRS})

# 4. Add your source files Must include every .cpp file that contains a class definition
add_executable(BlackjackAI 
    src/main.cpp 
    src/Deck.cpp 
    src/Card.cpp 
    src/Hand.cpp
)
# src/AI.cpp

# 5. Header Search Paths
# This tells the compiler where to find Card.h, Deck.h, etc.
target_include_directories(BlackjackAI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 6. Library Linking
# This tells the linker to attach the actual OpenCV and SQLite code.
# DO NOT put the /include directory here.
target_link_libraries(BlackjackAI 
    ${OpenCV_LIBS} 
    ${SQLITE3_LIBRARIES}
)