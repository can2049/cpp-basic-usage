add_executable(some_absl_string some_absl_string.cc)
target_link_libraries(some_absl_string absl::strings glog)

add_executable(use_status_or use_status_or.cc)
target_link_libraries(use_status_or absl::strings glog gflags absl::status absl::statusor)

add_executable(use_variant use_variant.cc)
target_link_libraries(use_variant absl::variant)

add_executable(strcat strcat.cc)
target_link_libraries(strcat absl::strings)

add_executable(use_clean_up use_clean_up.cc)
target_link_libraries(use_clean_up absl::cleanup)
