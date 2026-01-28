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

# 4. Add your source files
add_executable(BlackjackAI src/main.cpp)

# 5. Link libraries
target_link_libraries(BlackjackAI ${OpenCV_LIBS} ${SQLITE3_LIBRARIES})