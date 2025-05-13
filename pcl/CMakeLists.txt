find_package(GTest REQUIRED)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(compare_point compare_point.cc)
target_link_libraries(compare_point
glog  boost_system
${PCL_LIBRARIES}
)
