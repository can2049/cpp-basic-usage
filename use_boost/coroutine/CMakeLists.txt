find_package(Boost)

message("CMAKE_THREAD_LIBS_INIT => ${CMAKE_THREAD_LIBS_INIT}") # -lpthread

add_executable(co_hello_1 co_hello_1.cc)
target_link_libraries(co_hello_1 ${CMAKE_THREAD_LIBS_INIT} boost_system
                      boost_coroutine boost_context)

add_executable(co_hello_2 co_hello_2.cc)
target_link_libraries(co_hello_2 ${CMAKE_THREAD_LIBS_INIT} boost_system
                      boost_coroutine boost_context)

add_executable(co_hello_3 co_hello_3.cc)
target_link_libraries(co_hello_3 ${CMAKE_THREAD_LIBS_INIT} boost_system
                      boost_coroutine boost_context)

add_executable(co_hello_4 co_hello_4.cc)
target_link_libraries(co_hello_4 ${CMAKE_THREAD_LIBS_INIT} boost_system
                      boost_coroutine boost_context)
