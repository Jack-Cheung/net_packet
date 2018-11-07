set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
include_directories(${BOOST_INCLUDEDIR} ${PROJECT_SOURCE_DIR}/util/include )
add_executable(tcp-server main.cpp  ${PROJECT_SOURCE_DIR}/util/src/packet.cpp ${PROJECT_SOURCE_DIR}/util/src/header.cpp ${PROJECT_SOURCE_DIR}/util/src/param.cpp ${PROJECT_SOURCE_DIR}/util/src/global.cpp)
target_link_libraries(tcp-server ${Boost_LIBRARIES})