add_executable(clock clock.cc)
add_executable(time_interval time_interval.cc)
target_link_libraries(time_interval glog)

add_executable(current_time current_time.cc)
add_executable(time_hash time_hash.cc)
