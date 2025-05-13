set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g")

add_executable(move move.cc)
target_link_libraries(move glog)

add_executable(make_shared_from_instance make_shared_from_instance.cc)
target_link_libraries(make_shared_from_instance glog)

add_executable(inherit inherit.cc)
target_link_libraries(inherit glog)

add_executable(singletone singletone.cc)
target_link_libraries(singletone glog)

add_executable(template template.cc)

add_executable(pointer_polymorphism pointer_polymorphism.cc)

add_executable(copy_assign copy_assign.cc)
add_executable(base_class_method_call_base_class_method base_class_method_call_base_class_method.cc)
