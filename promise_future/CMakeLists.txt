find_package(Threads REQUIRED)

add_executable(promise promise.cc)
target_link_libraries(promise Threads::Threads glog)

add_executable(use_future use_future.cc)
target_link_libraries(use_future Threads::Threads glog)

add_executable(packaged_task packaged_task.cc)
target_link_libraries(packaged_task Threads::Threads glog)

add_executable(shared_future shared_future.cc)
target_link_libraries(shared_future Threads::Threads glog)
