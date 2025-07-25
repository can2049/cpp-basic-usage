add_executable(beast_https_demo beast_https_demo.cc)
target_link_libraries(beast_https_demo  ssl crypto pthread)

add_executable(beast_http_example beast_http_example.cc)
target_link_libraries(beast_http_example  ssl crypto pthread)
