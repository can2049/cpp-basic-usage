SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
# add_compile_options(-pthread)
add_executable(simple_producer_consumer simple_producer_consumer.cc)
add_executable(counter counter.cc)
