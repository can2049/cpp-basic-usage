add_executable(use_mutex use_mutex.cc)
target_link_libraries(use_mutex Threads::Threads)
