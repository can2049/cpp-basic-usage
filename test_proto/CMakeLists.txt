project(test_proto)

find_package(Protobuf REQUIRED)
file(GLOB ProtoFiles *.proto)

protobuf_generate_cpp(ProtoSources ProtoHeaders ${ProtoFiles})

add_library(${PROJECT_NAME} STATIC ${ProtoSources})
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES} )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(use_proto use_proto.cc)
target_link_libraries(use_proto ${PROJECT_NAME})
