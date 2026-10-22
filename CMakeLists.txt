cmake_minimum_required(VERSION 3.10)
project(Linux_CPP_Face_Detect)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 寻找 OpenCV 库
find_package(OpenCV REQUIRED)

# 包含 OpenCV 头文件路径
include_directories(${OpenCV_INCLUDE_DIRS})

# 生成可执行文件（名字叫 face_detect，源文件是 face_detect.cpp）
add_executable(face_detect face_detect.cpp)

# 链接 OpenCV 库
target_link_libraries(face_detect ${OpenCV_LIBS})