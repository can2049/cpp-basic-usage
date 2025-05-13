add_library(FileNotifier STATIC FileNotifier.cpp)
add_library(SafeFileWriter STATIC SafeFileWriter.cpp)

add_executable(reader_demo reader_demo.cc)
target_link_libraries(reader_demo FileNotifier SafeFileWriter Threads::Threads glog gflags absl::strings)

add_executable(writer_demo writer_demo.cc)
target_link_libraries(writer_demo FileNotifier SafeFileWriter Threads::Threads glog gflags absl::strings)
