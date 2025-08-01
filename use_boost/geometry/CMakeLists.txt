

add_executable(distance_example distance_example.cc)
target_link_libraries(distance_example boost_system glog)

add_executable(simplify_demo simplify_demo.cc)
target_link_libraries(simplify_demo boost_system glog)

add_executable(show_basic_shape show_basic_shape.cc)
target_link_libraries(show_basic_shape boost_system glog)

add_executable(linestring_distance_demo linestring_distance_demo.cc)
target_link_libraries(linestring_distance_demo boost_system glog)

add_executable(head_azimuth head_azimuth.cc)
target_link_libraries(head_azimuth boost_system glog)

add_executable(shape_buffer shape_buffer.cc)
target_link_libraries(shape_buffer boost_system glog)

add_executable(correct_shape correct_shape.cc)
target_link_libraries(correct_shape boost_system glog)
