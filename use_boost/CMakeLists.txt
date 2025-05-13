add_subdirectory(coroutine)

add_executable(use_circular_buffer use_circular_buffer.cc)
target_link_libraries(use_circular_buffer boost_system glog)

add_executable(signal2 signal2.cc)
target_link_libraries(signal2 boost_system glog)

add_subdirectory(rtree_demo)
add_subdirectory(bimap)
