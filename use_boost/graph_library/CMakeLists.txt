add_executable(bfs_example bfs_example.cc)
target_link_libraries(bfs_example boost_graph)

add_executable(graph_tutor graph_tutor.cc)
target_link_libraries(graph_tutor boost_graph)
