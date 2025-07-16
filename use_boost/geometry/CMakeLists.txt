

add_executable(distance_example distance_example.cc)
target_link_libraries(distance_example boost_system glog)

add_executable(simplify_demo simplify_demo.cc)
target_link_libraries(simplify_demo boost_system glog)
