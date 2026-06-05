cmake_minimum_required(VERSION 3.8)
project(semantic_mcl)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(rclcpp REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(semantic_msgs REQUIRED)

# add_library must be added first before target_include_directories
add_library(semantic_mcl_core STATIC
  src/beam_model.cpp
  src/motion_model.cpp
  src/particle_filter.cpp
  src/semantic_model.cpp
)

# allow the include to be exposed
target_include_directories(semantic_mcl_core PUBLIC
  include
)

target_link_libraries(semantic_mcl_core nlohmann_json::nlohmann_json)

# add the executable
add_executable(semantic_mcl_node_exec src/semantic_mcl_node.cpp)

target_link_libraries(semantic_mcl_node_exec semantic_mcl_core)

ament_target_dependencies(semantic_mcl_node_exec rclcpp nav_msgs sensor_msgs geometry_msgs semantic_msgs)

install(TARGETS semantic_mcl_node_exec
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  # for unit testing
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_semantic_mcl test/test_semantic_mcl.cpp)
  target_link_libraries(test_semantic_mcl semantic_mcl_core)
  set(ament_cmake_uncrustify_FOUND TRUE)
endif()

ament_package()
