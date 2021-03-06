cmake_minimum_required(VERSION 2.8.3)
project(track_odometry)

find_package(catkin REQUIRED COMPONENTS
	roscpp tf sensor_msgs std_msgs nav_msgs trajectory_msgs geometry_msgs rostest roslint
)

catkin_package(
	CATKIN_DEPENDS roscpp tf sensor_msgs std_msgs nav_msgs trajectory_msgs geometry_msgs rostest
)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


include_directories(${catkin_INCLUDE_DIRS} include)

add_executable(track_odometry src/track_odometry.cpp)
target_link_libraries(track_odometry ${catkin_LIBRARIES})

add_executable(tf_projection src/tf_projection.cpp)
target_link_libraries(tf_projection ${catkin_LIBRARIES})


roslint_cpp()

if(CATKIN_ENABLE_TESTING)
	find_package(rostest REQUIRED)

	catkin_add_gtest(tf_projection_test test/tf_projection_test.cpp)
	target_link_libraries(tf_projection_test ${catkin_LIBRARIES})

  roslint_add_test()
endif()

install(TARGETS
    track_odometry
    tf_projection
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

