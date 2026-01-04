add_executable(redirected_output redirected_output.cc)

add_library(io_monitor SHARED io_monitor.cc)
target_link_libraries(io_monitor pthread dl)

add_executable(random_file_io random_file_io.cc)
target_link_libraries(random_file_io pthread)

