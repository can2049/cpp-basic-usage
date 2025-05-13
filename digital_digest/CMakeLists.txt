add_executable(md5_demo md5_demo.cc)
target_link_libraries(md5_demo boost_system glog ssl crypto glog)
add_executable(validate_vin_code validate_vin_code.cc)
target_link_libraries(validate_vin_code glog)
