include_directories(${BOOST_INCLUDEDIR})
add_executable(tcp-server main.cpp)
target_link_libraries(tcp-server ${Boost_LIBRARIES})