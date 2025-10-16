cmake_minimum_required(VERSION 3.21)
project(llvm_tasks)

set(PASS_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/trace_pass/trace_pass)

add_subdirectory(trace_pass/)
add_subdirectory(graph_app/)