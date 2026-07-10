cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/msys64/ucrt64/include)
link_directories(C:/msys64/ucrt64/lib)

include_directories(subset_sum_solver/include)

add_executable(untitled
        subset_sum_solver/src/main.cpp
        subset_sum_solver/src/storage_engine.cpp
        subset_sum_solver/src/subset_sum_solver.cpp
)

target_link_libraries(untitled PRIVATE gmp gmpxx sqlite3)