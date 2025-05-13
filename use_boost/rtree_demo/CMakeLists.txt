add_executable(rtree_0 rtree_0.cc)
target_link_libraries(rtree_0 boost_system glog)

add_executable(rtree_1 rtree_1.cc)
target_link_libraries(rtree_1 boost_system glog)

add_executable(rtree_2 rtree_2.cc)
target_link_libraries(rtree_2 boost_system glog)

add_executable(rtree_3 rtree_3.cc)
target_link_libraries(rtree_3 boost_system glog)

add_executable(rtree_4 rtree_4.cc)
target_link_libraries(rtree_4 boost_system glog)

add_executable(rtree_5 rtree_5.cc)
target_link_libraries(rtree_5 boost_system glog)

add_executable(rtree_6 rtree_6.cc)
target_link_libraries(rtree_6 boost_system glog)

add_executable(rtree_7 rtree_7.cc)
target_link_libraries(rtree_7 boost_system glog)

add_executable(rtree_8 rtree_8.cc)
target_link_libraries(rtree_8 boost_system glog Threads::Threads rt)

add_executable(rtree_9 rtree_9.cc)
target_link_libraries(rtree_9 boost_system glog Threads::Threads)

add_executable(rtree_10 rtree_10.cc)
target_link_libraries(rtree_10 boost_system glog Threads::Threads)
