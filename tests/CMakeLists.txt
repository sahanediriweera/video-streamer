enable_testing()

add_executable(test_server_exec TestServer.cpp)
target_link_libraries(test_server_exec PRIVATE server_app common_lib)

add_executable(test_protocol_exec TestProtocol.cpp)
target_link_libraries(test_protocol_exec PRIVATE protocol_lib)
add_test(NAME ProtocolTest COMMAND test_proto_exec)

add_executable(test_client_exec TestClient.cpp)
target_link_libraries(test_client_exec PRIVATE video_lib common_lib)
add_test(NAME ClientTest COMMAND test_client_exec)