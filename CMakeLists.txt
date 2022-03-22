cmake_minimum_required(VERSION 3.20)
project(DataStructure C)

set(CMAKE_C_STANDARD 99)

add_executable(DataStructure main.c contiguous_list.h linked_list.h red_black_tree.h binary_search_tree.h avl_tree.h)
