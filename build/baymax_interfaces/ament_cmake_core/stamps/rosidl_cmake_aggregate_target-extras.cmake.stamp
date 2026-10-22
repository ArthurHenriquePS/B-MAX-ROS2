# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target baymax_interfaces::baymax_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${baymax_interfaces_TARGETS}.
if(baymax_interfaces_TARGETS AND NOT TARGET baymax_interfaces::baymax_interfaces)
  add_library(baymax_interfaces::baymax_interfaces INTERFACE IMPORTED)
  set_target_properties(baymax_interfaces::baymax_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${baymax_interfaces_TARGETS}")
endif()
