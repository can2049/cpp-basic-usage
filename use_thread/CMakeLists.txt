return ()
cmake_minimum_required(VERSION 3.16)

# Add executable for priority_thread_pool
add_executable(priority_thread_pool priority_thread_pool.cc)

# Link against the Threads package
target_link_libraries(priority_thread_pool Threads::Threads)

# Add compiler options
target_compile_options(priority_thread_pool PRIVATE -Wall -Wextra -O2)
