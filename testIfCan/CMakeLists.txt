project(tt)
find_package (Eigen3 3.3 REQUIRED NO_MODULE PATHS ../third_party/eigen-3.4.0)
link_libraries(Eigen3::Eigen)

add_executable(tt ./main.cpp)