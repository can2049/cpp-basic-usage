add_subdirectory(file_write_sdk)
add_executable(empty_dir empty_dir.cc)
add_executable(copy_dir copy_dir.cc)
add_executable(write_with_temp_file write_with_temp_file.cc)
add_executable(file_error_code file_error_code.cc)
add_executable(remove_demo remove_demo.cc)
add_executable(watch_single_file watch_single_file.cc)
add_executable(delete_while_read delete_while_read.cc)
target_link_libraries(delete_while_read PRIVATE pthread)
