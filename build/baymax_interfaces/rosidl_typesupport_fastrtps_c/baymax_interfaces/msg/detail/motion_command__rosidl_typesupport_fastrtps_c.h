// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice
#ifndef BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "baymax_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "baymax_interfaces/msg/detail/motion_command__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
bool cdr_serialize_baymax_interfaces__msg__MotionCommand(
  const baymax_interfaces__msg__MotionCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
bool cdr_deserialize_baymax_interfaces__msg__MotionCommand(
  eprosima::fastcdr::Cdr &,
  baymax_interfaces__msg__MotionCommand * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
size_t get_serialized_size_baymax_interfaces__msg__MotionCommand(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
size_t max_serialized_size_baymax_interfaces__msg__MotionCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
bool cdr_serialize_key_baymax_interfaces__msg__MotionCommand(
  const baymax_interfaces__msg__MotionCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
size_t get_serialized_size_key_baymax_interfaces__msg__MotionCommand(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
size_t max_serialized_size_key_baymax_interfaces__msg__MotionCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_baymax_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, baymax_interfaces, msg, MotionCommand)();

#ifdef __cplusplus
}
#endif

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
