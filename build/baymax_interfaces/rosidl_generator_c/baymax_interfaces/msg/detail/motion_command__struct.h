// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/motion_command.h"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__STRUCT_H_
#define BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'target'
// Member 'kind'
// Member 'name'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/MotionCommand in the package baymax_interfaces.
/**
  * Comando de movimento emitido pelo brain_node para o motor_bridge_node.
  *
  * target: qual MCU/subsistema. Um de {head, arm}
 */
typedef struct baymax_interfaces__msg__MotionCommand
{
  rosidl_runtime_c__String target;
  /// kind: tipo de comando. Um de {pan, tilt, head_gesture,
  ///       shoulder, elbow, grip, arm_gesture, stop, torque}
  rosidl_runtime_c__String kind;
  /// value: ângulo (0-180), garra (0-100) ou torque (0/1), quando numérico
  int32_t value;
  /// name: nome do gesto (ex. "nod", "wave"), quando kind for *_gesture
  rosidl_runtime_c__String name;
} baymax_interfaces__msg__MotionCommand;

// Struct for a sequence of baymax_interfaces__msg__MotionCommand.
typedef struct baymax_interfaces__msg__MotionCommand__Sequence
{
  baymax_interfaces__msg__MotionCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} baymax_interfaces__msg__MotionCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__STRUCT_H_
