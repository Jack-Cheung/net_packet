set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
include_directories(${BOOST_INCLUDEDIR} ${PROJECT_SOURCE_DIR}/util/include )
add_executable(tcp-client main.cpp ${PROJECT_SOURCE_DIR}/util/src/packet.cpp)
target_link_libraries(tcp-client ${Boost_LIBRARIES})
